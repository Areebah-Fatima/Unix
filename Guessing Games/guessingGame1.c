#include <stdio.h>

int main()
{
	// Declare all needed variables (hold max, min, and guess)
	int number = 0;
        int maxNum = 100;
	int minNum = 1;
	        
	char response = '!';

	// Repeat until user says your guess is equal	        
	while(response!='=')
	 {

	 	 //Guess will be half way between our current range
		 int number = (minNum + maxNum) /2;
					        
		 // Ask user if your guess is right (read their response)		
		 printf(" How about %d (<, >, =) :", (number));
		 scanf("%c", &response);
		 getchar();
									
		 // If true computer guess was too large				
		 if( response == '<'){		

		 	 // Find new number
			 maxNum = number-1;						    }										        
		
		 // If true computer guess was too small
		 if(response == '>'){

		 	 // Find new number
			 minNum = number+1;
											            }
									     		        }
	// Sucessfully guessed number(end game)
	printf("It was a good game!");
											
}

