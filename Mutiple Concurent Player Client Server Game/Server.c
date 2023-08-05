#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include<time.h>
#include <stdbool.h>

// Global constants to hold max number of possible words and max word length
#define WRDLIM 90000
#define MAXLEN 1000

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;

// Array to hold text file contents 
char *words[WRDLIM];
int numWrd = 0;

int main()
{
	key_t key;
	int msgid;

        // ftok to generate unique key
        key = ftok(getenv("HOME"), 1);
  
  	int counter=3;

	while (1) {

		 // msgget creates a message queue and returns identifier
	 	 msgid = msgget(key, 0666 | IPC_CREAT);
	  	 printf("Key %d Msgid %d\n", key, msgid);

	  	// msgrcv to receive message                                                                                                           
	  	msgrcv(msgid, &message, sizeof(message), 1, 0);

	        // fork to support multiple clients                                                                         
          	if (fork() == 0) {

	  		// Copy the client pid locally                                                                                                         
	  		int clientPid = atoi(message.mesg_text);

	  		// display the message                                                                                                                 
	  		printf("Data Received is : %s, client PID = %d \n", message.mesg_text, clientPid);

			
			// Open file for reading
	    		FILE *fp;
	    		char line[MAXLEN];
	    		fp = fopen("dictionary.txt", "r");
	    	
			// Let user know if file failed to open
			if(!fp) {
	      			printf("Failed to open the dicitionary\n");
	      			exit(1);
	    		}

	    		int i = 0;

	    		while(fgets(line, MAXLEN, fp)){
	      			words[i] = (char *) malloc (strlen(line)+1);
	      			strcpy(words[i], line);
	      			i++;
	    		}
	    		
			numWrd = i;
		
			// close
			fclose(fp);

			// random number
			srand(getpid()+time(NULL)+getuid());
			int randNum = rand() % numWrd; 
			
			// Choosen word length
			int length = strlen(words[randNum]) - 1;

        		// will hold display of  word
        		char gWord[length];

			// Fill display with stars
        		for (int i = 0; i < length; i++) {
            			gWord[i] = '*';
        		}

			int unexposed = 0;
			
			//assign dedicated project #s for communication to client
			int key2 = ftok(getenv("HOME"), clientPid+1);
    			int msgid2 = msgget(key2, 0666 | IPC_CREAT);
        		sprintf(message.mesg_text, "%d %d", counter, counter+1);
    			msgsnd(msgid2, &message, sizeof(message), 0);
    
			counter += 2;

			// for sending a message
                	int key3 = ftok(getenv("HOME"), clientPid + 2);
                	int msgid3 = msgget(key3, 0666 | IPC_CREAT);
		
			// receving a message
        		int key4 = ftok(getenv("HOME"), clientPid + 3);
        		int msgid4 = msgget(key4, 0666 | IPC_CREAT);
			
			//Message to user
                	printf("Word: %sLength: %d\n", words[randNum], length);
                	
			int missedCount = 0;
                	char charGuessed; 
			bool alreadyGuessed = false; 
			bool guessed = false;

			// Stop when no more stars left to unmask
			while (unexposed < length) {
 			
				int msgid4 = msgget(key4, 0666 | IPC_CREAT); 

				// If true it means unique letter guess
				if (!alreadyGuessed){

					sprintf(message.mesg_text, "%s %s", "(Guess) Enter a letter in word", gWord);
		        	}
				
				// Letter already guessed
				else {

			    		// Notify User
                            		sprintf(message.mesg_text, "%s %s", "The letter is already guessed.\n(Guess) Enter a letter in word", gWord);
                            		alreadyGuessed = false;
				} 
                	
				// send and recieve msg
				msgsnd(msgid3, &message, sizeof(message), 0);		
				msgrcv(msgid4, &message, sizeof(message), 1, 0);
				
				// user guessed letter
				charGuessed = message.mesg_text[0];
				guessed = false; 
			
				for (int i = 0; i < length; i++) {

					// If true it means guess letter is in word
					if (words[randNum][i] == charGuessed) {
						
						if (gWord[i] == '*') {
					        
							// unmask
							gWord[i] = charGuessed;
							unexposed++;
							guessed = true; 
						} 
						
						else {
					    		alreadyGuessed = true;
					    		guessed = true;
					   
                                       		}
					}					
				}

				// Guessed wrong update miss count
				if (!guessed) {
			    		missedCount++;
				}

				// Game end
				if (unexposed == length) {
					
					// Show use game analytics
					sprintf(message.mesg_text, "%s %s %s %d %s", "The word is ",gWord, ". You missed", missedCount, " time(s)");
                        		msgsnd(msgid3, &message, sizeof(message), 0);
				}		
			}
		
  		} 
	}
  	
    return 0;
}
