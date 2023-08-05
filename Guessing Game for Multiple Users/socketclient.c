#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    int c;

    // Buffer
    char recvBuff[1024];
    
    // Predefined structure: will provide AF_INET, SOCK_STREAM, etc.
    struct sockaddr_in serv_addr;

    // If true it means user forgot to add either port number or ip
    if(argc != 3)
    {
        // Notify user
	printf("\n Usage: %s <ip of server> <port #> \n",argv[0]);
        return 1;
    }

    
    memset(recvBuff, '0',sizeof(recvBuff));
    
    // Error with socket creation
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
    	// Notify user
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    
    serv_addr.sin_family = AF_INET;
    
    // Port number read from command line
    int port = atoi(argv[2]);
    // Translate host bytes order into network bytes order
    serv_addr.sin_port = htons(port);

    // Error
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
    	// Notify user
        printf("\n inet_pton error occured\n");
        return 1;
                 
    }
    
    // Error
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       // Notify user
       printf("\n Error : Connect Failed \n");
       return 1;
    }

        while (1) {

	    	// Read from the Stock
	    	n = read(sockfd, recvBuff, sizeof(recvBuff) - 1);
	    	// Overwrite /n in the read buffer
	    	recvBuff[n] = 0;
	    	// Display server message to the client
	    	fputs(recvBuff, stdout);

            	// Check if server sent an exit
	    	if (recvBuff[0] == 'Y') {
                    
		    // Tell user you're exiting
		    printf("\nServer sent Exit, exiting client! \n");
		    // close connection 
		    exit(0);
		    // exit
		    break; 
		}

		// Read the number guessed by the client
                int given;
                scanf("%d", &given);
		
		// Write the number guessed by the client to the server via the socket
                write(sockfd, &given, sizeof(int));

        }
    
    return 0;
}
