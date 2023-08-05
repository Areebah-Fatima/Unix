#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// No more than 100 people will play
#define MAXPLAYERS 100
// Limit name lengths to 100 character
#define MAXLEN 100

// Arrays to store the player names and balances
char names[MAXPLAYERS][MAXLEN];
double balances[MAXPLAYERS];
int numPlayers = 0; // Set when actual player data is loaded

// Search the player names array and return the index if specified name is found
// Return -1 otherwise.
int findIndex(char *name) {
        for(int i=0; i<numPlayers; i++)
            if(strcmp(name, names[i]) == 0)
               return i;

        return -1;
}

// Will be using binary data file gifts1.dat to read and store the results.
// Will use system calls open() write() read() seek() close()
int main(int argc, char *argv[]) {

	int NLen = 0;
	int fd;
	double  money;
	char* player [100];

	// If we go into the if statement it means we have started a new game
	if(argv[1][0] == 'n' && argv[1][1] == 'e' && argv[1][2] == 'w')
	{
		// Open file and set permission
		fd = open("gifts1.dat", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
		
		// argv[0] -> program name	argv[1] -> new\0	argv[3] -> balance
		for(int i = 2; i<argc; i++){

			// If true we're dealing with a player name (all even i argv[i] has name)
			if(i%2 ==0){
				
				// Copy command line argument to array holding names
				strcpy(names[numPlayers], argv[i]);
			}
			else{
				// Otherwise we dealing with argv[i+1] (assuming i is even) these hold money amount
				
				//Atof is character pointer (convert type)
				// Store balance and increment player count
				balances[numPlayers] = atof(argv[i]);
				numPlayers++;
			}
		}
	}


	// This program will first create fd file (done above), if we're not playing new game set file permissions differently
	else
	{

		int y;
		
		// Open for read only
		fd = open("gifts1.dat", O_RDONLY);

		NLen = 0;
		char PlayerName[100];

		// New games will refreash array so we need to repopulate them
		// Read from bin file until you reach the end of a players name (held in NLen)
		while((read(fd, &NLen, sizeof(int))) > 0)
		{

			// Read names in and populate name array
			read(fd, PlayerName, NLen * sizeof(char) + 1);
			strcpy(names[numPlayers], PlayerName);

			// Read balances in and populate array
			read(fd, &money, sizeof(double));

			balances[numPlayers] = money;
			
			// Keep track of how many players 
			numPlayers++;
		}
		
		// CLOSE
		close(fd);
		
		// Use helper function to find where player is
		int index = findIndex(argv[1]);

		// True if player has been created before
		if(index != -1)
		{
			// Remove amount loss from player balance
			balances[index] = balances[index] - atof(argv[2]);

		}

		int PlayerC = 0;
		
		// See how many people to give money to
		for(int i=3; i<argc; i++)
		{
			PlayerC++;
		}


		// find out how much money to give player
		money = atof(argv[2])/PlayerC;

		// Update the balances of all player recieving money
		for(int i = 3; i<argc; i++)
		{
			index = findIndex(argv[i]);
			if(index != -1)
			{
				balances[index] = balances[index] + money;
			}
		}
		
		// Open file one last time to write updated data
		fd = open("gifts1.dat", O_WRONLY);

	}

	// Loop through all players
	for(int i=0; i< numPlayers; i++)
	{
		NLen = strlen(names[i]);
		
		// Write names and balances
		write(fd, &NLen, sizeof(int));
		write(fd, &names[i], sizeof(char) * (strlen(names[i])+1));
		write(fd, &balances[i], sizeof(double));

		// Formatted Print
		printf("%10s: %5.2lf\n", names[i], balances[i]);
	}
	
	// CLOSE
	close(fd);
}
