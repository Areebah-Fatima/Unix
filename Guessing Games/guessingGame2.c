#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {

	// Seed random number
	srand(time(0));

	// ActualNum will be a random number from 1 to 0
	int actualNum = (rand() % 100) + 1;
	int userGuess;
	int count = 1;
	
	// Prompt user for guess and store it
	printf("Ok, I am ready! \nEnter your guess: ");
	scanf("%d", &userGuess);

	// Loop will end when user guess equals actual number
	while( userGuess != actualNum){
		
		// Increment count for guess number
		count++;
		
		// If true it means user guessed number too big
		if(userGuess > actualNum){

			// Notify them and read in new guess
			printf("Go lower!\nEnter your guess: ");
			scanf("%d", &userGuess);
		}

		// If true it means user guess is too small
		else if (userGuess < actualNum){

			// Notify them and read in new guess
			printf("Go higher!\nEnter your guess: ");
			scanf("%d", &userGuess);
		}
	}

	// User guessed number (end game)
	printf("You got it in %d tries!", count);
}

