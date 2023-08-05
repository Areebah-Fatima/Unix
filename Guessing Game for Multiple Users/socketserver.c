#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>

// exit function from server4 activity
void pexit(char *errmsg) {
        
	// Error message then exit
	fprintf(stderr, "%s\n", errmsg);
        exit(1);
}

int main(int argc, char *argv[])
{
    
    int listenfd = 0, connfd = 0;
    
    // Will use this struct later in code for thing like AF_INET, SOCK_STREAM, etc
    struct sockaddr_in serv_addr;

    // Buffer to hold inputs and responses
    char buffer[1025];
    time_t ticks;

    // If true error has occurred notify user and ecit
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
                pexit("socket() error.");
    }

    // Fill memory block
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buffer, '0', sizeof(buffer));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int port = 4999;
    //int port = atoi(argv[1])-1;
       
    // Look for available port, bind/connect to it, and tell user the port number   
    do {
        port++;
        serv_addr.sin_port = htons(port);
    } while (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0);
        printf("bind() succeeds for port #%d\n", port);

    // Error handling
    if (listen(listenfd, 10) < 0)
                pexit("listen() error.");

    int counter=0;

    while(1)
    {
    		// accept client request for connection
                connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
                counter++;

		// Added to support multiple client runs
                if (fork())
                        continue;

		// C strings will be passed based on scenario
                char startDisplay[] = "Enter your guess from 1-100:\n";
                char smaller[] = "Go lower!\n";
                char larger[] = "Go higher!\n";
		char exit[] = "Xit";
		char bufferTx[1000];
                int response;
                bool keepGoing = true;
		
		// Seed
		srand(time(NULL));
                // Will hold random number
		int randNum = rand() % 100 + 1;
                
		// Number of tries
		int round = 1; 

		//printf("number is %d\n", randNum);
                
		printf("Connected client. Serving client #%d.\n", counter);
                
		// while not guessed
		while (keepGoing) {
			
			// Write guess prompt and read response
			//printf("Ok, I am ready! \n");
                        write(connfd, startDisplay, strlen(startDisplay));
                        read(connfd, &response, sizeof(int));
                        
			//printf("User gave number %d\n", response);
			
			// If true user need to guess smaller numbers, print appropriate message (write)
                        if(response > randNum){

				// Increment try counter
                                round++;
				write(connfd, smaller, strlen(smaller));
                        }
                        
			// If true user needs to guess larger numbers, print approriate message (write)
			else if(response < randNum){

			       // Increment try counter
                               round++; 
			       write(connfd, larger, strlen(larger));
                        }

			// If true it means user guessed right number
                        else if(response == randNum){
                                
				// Store string in buffer
				sprintf(bufferTx, "You got it in %d tries", round);          			
				// flag for game continue set to false
				keepGoing = false;

                                write(connfd, bufferTx, strlen(bufferTx));
           			
				// close
				close(connfd); 
			}
                }
        sleep(1);
     }
}
