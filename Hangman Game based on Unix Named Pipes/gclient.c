#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MAXLEN 1000

int main(int argc, char *argv[]) {	
	
	// User didn't give server, notify them
	if (argc !=2) {
		puts("Usage: qclient <server-fifo-name>");
		exit(1);
	}


    char clientfifo[MAXLEN];

	// Create unique pipename
        sprintf(clientfifo, "/tmp/%s-%d", getenv("USER"), getpid());
	mkfifo(clientfifo, 0600);
	chmod(clientfifo, 0622);
	
	// Open argv[1] for writing, send clientfifo
	FILE *fp = fopen(argv[1], "w");
	
	// Add \n after clientfifo to avoid potential race conditions in server
	fprintf(fp, "%s\n", clientfifo);
	fclose(fp);

	// Open clientfifo for reading and read the quote & print in the screen - improve your life! :-)
	FILE *clientfp = fopen(clientfifo, "r");
	
	// Read the new server-fifo, then open it for writing! as serverfp
	char serverfifo[MAXLEN];
	fscanf(clientfp, "%s", serverfifo);
	
	char Input[MAXLEN];

	fgets(Input, MAXLEN, clientfp);
	FILE *serverfp = fopen(serverfifo, "w");
	
	// infite loop
	while (1)
	{	
		// if data is sent
		if(fgets(Input, MAXLEN, clientfp)){
		puts(Input);
		
		// Process letter
		char letter;
		scanf("%c", &letter);
		
		// get user response
		char response[10];
		fgets(response, 10, stdin);

		fprintf(serverfp, "%c\n", letter);
		fflush(serverfp);
		}

		// esle break inifite loop
		else{

			break;
		}
	}

	// Clean up
	fclose(clientfp);

	unlink(clientfifo);
}
