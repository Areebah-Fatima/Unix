#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

// Global constants
#define WRDLIM 90000
#define MAXLEN 1000

// will hold all words in dictionary file
char *words[WRDLIM];
int numWrd = 0;

int main() {

	char line[MAXLEN];
	
	// read dictionary file to array of words
	FILE *fp  = fopen("dictionary.txt", "r");
	
	//If true means we had issue with reading the file, notify user 
	if(!fp){
		puts("dictionary had issues openning");
		exit(1);
	}
	int i=0;
	
	while(fgets(line, MAXLEN, fp)){

                words[i] = (char *) malloc (strlen(line)+1);
		strcpy(words[i], line);
                i++;
    	}
    	numWrd = i;

	//show number of words read
	printf("%d words read \n", numWrd);

	// random number
	srand(getpid()+time(NULL)+getuid());

	// create named unique pie
   	char filename[MAXLEN];
	sprintf(filename, "/tmp/%s-%d", getenv("USER"), getpid());
   	mkfifo(filename, 0600);
	chmod(filename, 0622);	// set access
	printf("SEND request to %s\n", filename);


	while (1) {
       
		// open for reading
		FILE *fp = fopen(filename, "r");
		
		// if true issue with reading file
		if(!fp){
		
			printf("FIFO %s cannot be opened\n", filename);
			exit(2);
		}


		while(fgets(line, MAXLEN, fp)){

			char *cp = strchr(line, '\n');
			if(cp)
				*cp = '\0';


		    if(fork() == 0){
			FILE *clientfp = fopen(line, "w");
			
			// another unique pipename
			char serverfifo[MAXLEN];
			sprintf(serverfifo, "/tmp/%s-%d", getenv("USER"), getpid());
                    	mkfifo(serverfifo, 0600);
			chmod(serverfifo, 0622);
			
			fprintf(clientfp, "%s\n", serverfifo);
			fflush(clientfp);

			FILE *serverfp = fopen(serverfifo, "r");

			// will hold word to guess
			char gWord[MAXLEN];

			//fprintf("Word to guess %s", gWord);
			
			// hold guesses missed
			int Misses = 0;
			
			// copy word to guess from random choice of word array
			strcpy(gWord, words[rand() % numWrd]);

			printf("word to guess %s %d\n", gWord, strlen(gWord));

			char CharacterG;

			// holds word to guess length
			int n = 0;
			n = strlen(gWord)-1 ;

			//fprintf(clientfp, "here is work length %d\n", n);

			int unexposed = 0;

			//printf("unexposed %d\n", unexposed);
			char display[n];
			
			// fill display with '*'
			for(int i=0; i<n; i++){
        			display[i] = '*';
			}

			// while user has not yet revealed words
			while (unexposed <  n) {
				

				fprintf(clientfp, "(GUESS) Enter a letter in word %s > \n", display);
				fflush(clientfp);
				
				// holds user response
				char response[10];
				fscanf(serverfp, "%s", response);
				CharacterG = response[0];

				bool isfound = false;

				// check if user is right
				for(int i=0; i<n; i++){
					
					// if true it means user was right
					if(CharacterG == gWord[i]){
					
						isfound = true;

						// if word is already being displayed it means user already guessed it
						if(CharacterG == display[i]){
							fprintf(clientfp, "%c is already in the word. ", CharacterG);
							fflush(clientfp);
							break;
						}
						else{
							display[i] = CharacterG;
							unexposed++;
						}
			        }
		        }

			// not correctly guessed
			if(!isfound){

				// increment missed count
				fprintf(clientfp, "%c is not in the words. ", CharacterG);
				fflush(clientfp);
				Misses++;
			}
	        
			/*if(unexposed == n){
				fprintf( clientfp, "the word you've been trying to guess is %s", gWord);
				fflush(clientfp);
				fprintf(clientfp, "you miss guessed  %d times \n", Misses);
			        fflush(clientfp);
			}*/
		}
		
		// we have reached the end of the game print results to user
		fprintf( clientfp, "the word you've been trying to guess is %s", gWord);
		fflush(clientfp);
		fprintf(clientfp, "you miss guessed  %d times \n", Misses);
		fflush(clientfp);
		exit(0);
 }
 }

fclose(fp);
 }
 }
