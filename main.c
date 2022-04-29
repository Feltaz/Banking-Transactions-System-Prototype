#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <glib.h>

#include "shared.h"
#include "bills.h"
#include "accounts.h"
#include "transactions.h"
#include "banking_terminal.h"

#define CLOSE_COMMAND "close"
#define OPERATION_DEBIT "debit"
#define OPERATION_CREDIT "credit"
#define REQUEST_PARTS 3

GQueue* client_socket_fd_queue; // Queue to store client sockets
pthread_t thread_pool[20];
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for client sockets queue
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER; // Condition for client sockets queue

int handle_request(char* buffer){
    // The request's syntax is: [operation:ref:value]
    // operation is either credit or debit
    // ref is the account's reference
    // value is the transaction's value

    char* request_parts[REQUEST_PARTS]; // Array to stored request parts
    char* part = NULL; // Buffer to hold request part
    char* delimiter = ":"; // The request delimiter character
    char* operation = NULL;
    int ref;
    float value;
    int n = 0; // Length of request_parts

    // If the request is the close command, send a close command in the response
    if(!strncmp(buffer, CLOSE_COMMAND, 256)){
        bzero(buffer, 256);
        strncpy(buffer, CLOSE_COMMAND, 256);

        return 0;
    }

    // Parse request
    part = strtok(buffer, delimiter);
    while(part){
        if(n >= REQUEST_PARTS){
            bzero(buffer, 256);
            strncpy(buffer, "Too many parameters provided! \n", 256);

            return -1;
        }

        request_parts[n] = part;
        n++;

        part = strtok(NULL, delimiter);
    }

    if(n < 2) {
        strncpy(buffer, "Too few parameters provided! \n", 256);
        return -1;
    }

    operation = request_parts[0];
    ref = atoi(request_parts[1]);
    if(n > 2){
        value = atof(request_parts[2]);
    }

    printf("--> client request: %s %d %.2f \n", operation, ref, value);

    // Handle request
    if(!strncmp(operation, OPERATION_DEBIT, sizeof(buffer))){
        switch(debit(ref, value)){
            case -2:
                strncpy(buffer, "Transaction failed! Amount exceeded negative withdrawal threshold. \n", 256);
                return -1;
            case -1:
                strncpy(buffer, "Invalid reference! No account was found. \n", 256);
                return -1;
            default:
                // Get success message with billing information if the account has bills to pay
                {char* message = get_transaction_success_message(ref);
                strncpy(buffer, message, 256);
                free(message);
                return 0;}
        }
    }
    else if(!strncmp(operation, OPERATION_CREDIT, sizeof(buffer))){
        switch(credit(ref, value)){
            case -1:
                strncpy(buffer, "Invalid reference! No account was found. \n", 256);
                return -1;
            default:
                // Get success message with billing information if the accounts has bills to pay
                {char* message = get_transaction_success_message(ref);
                strncpy(buffer, message, 256);
                free(message);
                return 0;}
        }
    }
    else{
        strncpy(buffer, "Invalid operation! \n", 256);
        return -1;
    }
}

void* handle_connection(void * arg) {
    char buffer[256]; // buffer to store socket message
    int* client_socket_ptr; // pointer to client socket

    while(1){
        // Try to get a client socket from client sockets queue
        pthread_mutex_lock(&queue_mutex);
        client_socket_ptr = g_queue_pop_head(client_socket_fd_queue);
        // If no client socket is in the queue, block until a socket is received
        if(client_socket_ptr == NULL) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
            client_socket_ptr = g_queue_pop_head(client_socket_fd_queue);
        }
        pthread_mutex_unlock(&queue_mutex);

        if(client_socket_ptr != NULL){
            // Loop for client session
            while(1){
                bzero(buffer, sizeof(buffer));

                // Read from client
                if(read(*client_socket_ptr, buffer, sizeof(buffer)) < 0){
                    printf("Failed to receive message! \n");
                    exit(-1);
                }

                handle_request(buffer);

                // Write to client
                if(write(*client_socket_ptr, buffer, strlen(buffer)) < 0){
                    printf("Failed to send message! \n");
                    exit(-1);
                }

                // End session loop on receiving close command
                if(!strncmp(buffer, CLOSE_COMMAND, sizeof(buffer))){
                    break;
                }
            }

            // Close client socket
            close(*client_socket_ptr);
            free(client_socket_ptr);
        }
    }

    return NULL;
}

int main()
{
    pthread_t banking_system_thread; // thread to handle banking system menu
    pthread_create(&banking_system_thread, NULL, main_banking_menu, NULL);

    int socket_fd, client_socket_fd; // variables to store server and client file descriptors
    struct sockaddr_in serv_addr; // variable to store server address

    // Initialize client sockets queue
    client_socket_fd_queue = g_queue_new();

    // Start worker threads
    for(int i = 0; i < 20; i++){
        pthread_create(&thread_pool[i], NULL, handle_connection, NULL);
    }

    // Create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if(!socket_fd){
        printf("Failed to create socket! \n");
        exit(-1);
    }

    // Create server address
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(9800);

    // Bind socket to server address and start listening
    if(bind(socket_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0){
        printf("Failed to bind socket! \n");
        exit(-1);
    }
    listen(socket_fd, 20);

    // Main server loop to receive client connections
    while(1) {
        // Receive client connection
        client_socket_fd = accept(socket_fd, NULL, NULL);
        if(client_socket_fd < 0){
            printf("Failed to connect client! \n");
            exit(-1);
        }

        // Add client socket to client sockets queue
        int* client_socket_fd_ptr = (int*)malloc(sizeof(int));
        *client_socket_fd_ptr = client_socket_fd;

        pthread_mutex_lock(&queue_mutex);
        g_queue_push_tail(client_socket_fd_queue, client_socket_fd_ptr);
        pthread_cond_signal(&queue_cond);
        pthread_mutex_unlock(&queue_mutex);
    }

    // Close server socket
    close(socket_fd);

    return 0;
}
