#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <glib.h>

#include "shared.h"
#include "bills.h"
#include "accounts.h"
#include "transactions.h"

void print_account(ACCOUNT* account){
    printf("Account Details: \n");
    printf("Reference: %d\n", account->ref);
    printf("Value: %.2f\n", account->value);
    printf("State: %s\n", account->state);
    printf("Debit Threshold: %.2f\n", account->debit_threshold);
    printf("\n");
}

void print_transaction(TRANSACTION* transaction){
    printf("Transaction Details: \n");
    printf("Reference: %d\n", transaction->ref);
    printf("Transaction: %s\n", transaction->transaction);
    printf("Value: %.2f\n", transaction->value);
    printf("Resultat: %s\n", transaction->result);
    printf("State: %s\n", transaction->state);
    printf("\n");
}

void print_bill(BILL* bill){
    printf("Bill Details: \n");
    printf("Reference: %d\n", bill->ref);
    printf("Debt: %.2f\n", bill->debt);
    printf("\n");
}

const char* CLOSE_COMMAND = "close";

int handle_client_request(char* buffer){
    const char* OPERATION_DEBIT = "debit";
    const char* OPERATION_CREDIT = "credit";
    const int REQUEST_PARTS = 3;

    char* request_parts[REQUEST_PARTS];
    char* part = NULL;
    char* delimiter = ":";
    char* operation = NULL;
    int ref;
    float value;
    int n = 0;

    if(!strncmp(buffer, CLOSE_COMMAND, 256)){
        bzero(buffer, 256);
        strncpy(buffer, CLOSE_COMMAND, 256);

        return 0;
    }

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

    printf("%s %d %.2f \n", operation, ref, value);
    if(!strncmp(operation, OPERATION_DEBIT, sizeof(buffer))){
        switch(debit(ref, value)){
            case -2:
                strncpy(buffer, "Transaction failed! Amount exceeded negative withdrawal threshold. \n", 256);
                return -1;
            case -1:
                strncpy(buffer, "Invalid reference! No account was found. \n", 256);
                return -1;
            default:
                strncpy(buffer, "Transaction successful! \n", 256);
                return 0;
        }
    }
    else if(!strncmp(operation, OPERATION_CREDIT, sizeof(buffer))){
        switch(credit(ref, value)){
            case -1:
                strncpy(buffer, "Invalid reference! No account was found. \n", 256);
                return -1;
            default:
                strncpy(buffer, "Transaction successful! \n", 256);
                return 0;
        }
    }
    else{
        strncpy(buffer, "Invalid operation! \n", 256);
        return -1;
    }
}

int main()
{
    int socket_fd, client_socket_fd;
    struct sockaddr_in serv_addr;
    char buffer[256];

    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if(!socket_fd){
        printf("Failed to create socket! \n");
        exit(-1);
    }

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(9800);

    if(bind(socket_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0){
        printf("Failed to bind socket! \n");
        exit(-1);
    }

    listen(socket_fd, 5);
    client_socket_fd = accept(socket_fd, NULL, NULL);
    if(client_socket_fd < 0){
        printf("Failed to connect client! \n");
        exit(-1);
    }

    while(1){
        bzero(buffer, sizeof(buffer));

        if(read(client_socket_fd, buffer, sizeof(buffer)) < 0){
            printf("Failed to receive message! \n");
            exit(-1);
        }

        printf("%s \n", buffer);

        handle_client_request(buffer);

        if(write(client_socket_fd, buffer, strlen(buffer)) < 0){
            printf("Failed to send message! \n");
            exit(-1);
        }

        if(!strncmp(buffer, CLOSE_COMMAND, sizeof(buffer))){
            break;
        }
    }

    close(socket_fd);
    close(client_socket_fd);

    return 0;
}
