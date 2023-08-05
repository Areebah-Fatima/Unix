#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
  
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;
  
int main(int argc, char *argv[])
{
    	key_t key;
    	int msgid;
  
	// ftok to generate unique key
  	key = ftok(getenv("HOME"), 1);
  
	// We will use client PID to create a unique key 
	int pid = getpid();
    	message.mesg_type = 1;
	sprintf(message.mesg_text, "%d", pid);
    
     	// msgget creates a message queue and returns identifier
	msgid = msgget(key, 0666 | IPC_CREAT);

    	// msgsnd to send message
    	msgsnd(msgid, &message, sizeof(message), 0);
    	
	// generate another key
    	int key2 = ftok(getenv("HOME"), pid+1);
    	int msgid2 = msgget(key2, 0666 | IPC_CREAT);
        
	// recieve msg and destroy
	msgrcv(msgid2, &message, sizeof(message), 1, 0);
    	msgctl(msgid2, IPC_RMID, NULL);

    	// recieve message
	int key3 = ftok(getenv("HOME"), pid+2);
	int msgid3 = msgget(key3, 0666 | IPC_CREAT);
        
	// send message
	int key4 = ftok(getenv("HOME"), pid+3);
        int msgid4 = msgget(key4, 0666 | IPC_CREAT);
	
	// Will hold guessed character
	char guess[1];

	// While game has not ended
	while (1) { 
	
    		// receive message
		int msgid3 = msgget(key3, 0666 | IPC_CREAT);
		msgrcv(msgid3, &message, sizeof(message), 1, 0);
    		printf("%s \n", message.mesg_text);
	
		// if word guessed
		if (strcmp(message.mesg_text,"Completed!") == 0) {
			
			int msgid3 = msgget(key3, 0666 | IPC_CREAT);
			msgrcv(msgid3, &message, sizeof(message), 1, 0);
                	printf("%s \n", message.mesg_text);
			break;

		}

		// send message
		gets(guess);
		sprintf(message.mesg_text, "%s", guess);
		msgsnd(msgid4, &message, sizeof(message), 0);

	}
	
	msgctl(msgid3, IPC_RMID, NULL);
    	return 0;
}
