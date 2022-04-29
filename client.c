#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CLOSE_COMMAND "close"

void main_client_menu(char* buffer) {
	char choice;
	int ref;
	float value;

	system("clear");

	printf("----------------------------------------------------------------------------------\n");
	printf("================================== Terminal Banque ===============================\n");
	printf("----------------------------------------------------------------------------------\n");
	printf("                              Veuillez choisir une option                         \n");
	printf("                                                                                  \n");
	printf("                                   1 - Debiter                                    \n");
	printf("                                   2 - Créditer                                   \n");
	printf("                                   3 - Quitter                                    \n");
	printf("----------------------------------------------------------------------------------\n");

	do{
	    scanf("%c", &choice);
	}
	while(choice != '1' && choice != '2' && choice != '3');

	system("clear");

	switch(choice){
	case '1':
	    printf("Saisir la référence du compte: ");
	    scanf("%d", &ref);
	    printf("Saisir la valeur de la transaction: ");
	    scanf("%f", &value);

	    sprintf(buffer, "debit:%d:%.2f", ref, value);
	    break;
	case '2':
	    printf("Saisir la référence du compte: ");
	    scanf("%d", &ref);
	    printf("Saisir la valeur de la transaction: ");
	    scanf("%f", &value);

	    sprintf(buffer, "credit:%d:%.2f", ref, value);
	    break;
	case '3':
	    sprintf(buffer, "close");
	    break;
    }
}


int main(){
	int socket_fd;
	struct sockaddr_in serv_addr;
	char buffer[256];
	
	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	
	if(!socket_fd){
		printf("Failed to create socket!");
		exit(-1);
    	}
    	
    	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	//inet_aton("20.199.72.82", &(serv_addr.sin_addr));
    	serv_addr.sin_addr.s_addr = INADDR_ANY;
    	serv_addr.sin_port = htons(9800);
    	
    	if(connect(socket_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0){
    		printf("Failed to connect to server socket!");
    		exit(-1);
   	}
   	
   	while(1){
   		bzero(buffer, sizeof(buffer));
	   	/*printf("Type command: ");
	   	fgets(buffer, sizeof(buffer), stdin);	
	   	buffer[strcspn(buffer, "\r\n")] = '\0'; // Clear the newline character at the end of the input caused by fgets()*/
	   	
	   	main_client_menu(buffer);
	   	
	   	if(write(socket_fd, buffer, strlen(buffer)) < 0){
	   		printf("Failed to send message!");
	   		exit(-1);
	   	}
	   	
	   	bzero(buffer, sizeof(buffer));
	   	if(read(socket_fd, buffer, sizeof(buffer)) < 0){
			printf("Failed to receive message!");
	   		exit(-1);
	   	}
	   	
	   	if(!strncmp(buffer, CLOSE_COMMAND, sizeof(buffer))){
		    break;
		}
	   	
	   	printf("%s\n", buffer);
	   	printf("Press any key to continue...\n");
	   	getchar();
	   	getchar();

   	}
   	
   	close(socket_fd);
   	
   	return 0;
}