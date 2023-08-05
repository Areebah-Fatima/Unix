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

// Define our low end object (struct)
struct Player {
        char name[MAXLEN];
        double balance;
};

// Struct is like class - we can use an array of struct (we can use like an array of objects).
struct Player players[MAXPLAYERS];
int numPlayers = 0; // Set when actual player data is loaded

// Search the player names array and return the index if specified name is found
// Return -1 otherwise.
int findIndex(char *name) {
        for(int i=0; i<numPlayers; i++)
            if(strcmp(name, players[i].name) == 0)
               return i;

        return -1;
}


// Will be using binary data file gifts2.dat to read and store the results.
// Will use standard IO fopen() fwrite() fread() fseek() fclose()
int main(int argc, char *argv[]) {
	
	// Declare file pointer
	FILE *filep;
	double  money;
	char player [100];

	// If we go into the if statement it means we have started a new game
	if(argv[1][0] == 'n' && argv[1][1] == 'e' && argv[1][2] == 'w')
	{
		// Open file and set permission to write binary file
		filep = fopen("gifts2.dat", "wb");
		
		// argv[0] -> program name	argv[1] -> new\0	argv[3] -> balance
		for(int i = 2; i<argc; i++){

			// If true we're dealing with a player name (all even i argv[i] has name)
			if(i%2 ==0){
				
				// Copy command line argument to our object
				strcpy(players[numPlayers].name, argv[i]);
			}
			else{
				// Otherwise we dealing with argv[i+1] (assuming i is even) these hold money amount
				
				//Atof is character pointer (convert type)
				// Store balance and increment player count
				players[numPlayers].balance = atof(argv[i]);
				numPlayers++;
			}
		}
	}


	// This program will first create filep file (done above), if we're not playing new game set file permissions differently
	else
	{

		// Open for read binary file only
		filep = fopen("gifts2.dat", "rb");
		
		// Read in numPlayers fread( pointer, size in bytes of each element, number of elements, FILE object)
		numPlayers = fread(players, sizeof(struct Player), MAXPLAYERS, filep); 
		
		// CLOSE
		fclose(filep);
		
		// Use helper function to find where player is
		int index = findIndex(argv[1]);

		// True if player has been created before
		if(index != -1)
		{
			// Remove amount loss from player balance (using struct object definition)
			players[index].balance = players[index].balance - atof(argv[2]);

		}

		int PlayerC = 0;
		
		// See how many people to give money to
		for(int i=3; i<argc; i++)
		{
			PlayerC++;
		}

		// Finnd out how much money to give player
		money = atof(argv[2])/PlayerC;

		// Update the balances of all player recieving money
		for(int i = 3; i<argc; i++)
		{
			index = findIndex(argv[i]);
			if(index != -1)
			{
				players[index].balance = players[index].balance + money;
			}
		}
		
		// Open file one last time to write updated data
		filep =  fopen("gifts2.dat", "wb");

	}

	// fwrite( pointer, size of each element, number of elements, FILE object)
	fwrite(players, sizeof(struct Player), numPlayers, filep);

	// Loop through all players
	for(int i=0; i< numPlayers; i++)
	{
		
		// Write names and balances
		// Formatted Print
		printf("%10s: %5.2lf\n", players[i].name, players[i].balance);
	}
	
	// CLOSE
	fclose(filep);
}
