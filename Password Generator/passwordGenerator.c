#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

// Pass command line arguments to main
int main(int argc, char** argv)
{
    int minLen, maxLen, minLower, minUpper, minDigits, minSpecial;
    int count =0;

    // Seed generator
    srand(time(NULL) + getpid());
    
    // Convert all command line arguments to integers
    minLen = atoi(argv[1]);
    maxLen = atoi(argv[2]);
    minLower = atoi(argv[3]);
    minUpper = atoi(argv[4]);
    minDigits = atoi(argv[5]);
    minSpecial = atoi(argv[6]);

    // Make sure min is large enough to meet password requirements
    int newMin = minSpecial+minDigits+minUpper+minLower;
    int randomLen = 0;

    // If true we will use minLen to generate random length (or else we will end up with a password of lesser length than specified minLen)
    if( newMin < minLen ){
	
	 randomLen = (rand()%(maxLen-minLen+1))+minLen;
    }

    // If true use newMin to generate random length (so password size range is large enough to meet minimum requirements) 
    else{
	 randomLen = (rand()%(maxLen-newMin + 1)) + newMin;
    }

    // C string to hold password
    char password[randomLen];

    // Array to hold all possible password entries
    char options[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
                        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's','t',
                            'u', 'v', 'w', 'x', 'y', 'z', '!', '@', '#', '$', '%', '^', '&', };

    // Loop to fill password with minimum number of lower case letters
    for(int i=0; i<minLower; i++){
 
        // Use index range that includes lower case letters
        int index = (rand()%(34-9 + 1)) + 9;

	// Add randomly selected character to our c string
        password[count] = options[index];
        count++;
    }
    
    // Loop to fill password with minimum number of upper case letters
    for(int j=0; j<minUpper; j++){

        // Use range index for letters
        int index = (rand()%(34-9 + 1)) + 9;

	// Add randomly selected letter to C string (use toupper to make character upper case)
        password[count] = toupper(options[index]);
        count++;

    }

    // Loop to fill password with minimum number of numbers
    for(int l=0; l<minDigits; l++){

        // Use options array index to determine random generator range (0-8)
        int index = (rand()%(8-0 + 1)) + 0;

	// Add randomly selected number to C string
        password[count] = options[index];
        count++;

    }

    // Loop to fill password with the minimum number of special characters
    for(int p=0; p<minSpecial; p++){

        // Use options array indexes to determine random generator range
        int index = (rand()%(41-35 + 1)) + 35;

	// Add randomly selected number to C string
        password[count] = options[index];
        count++;

    }

    // Execute loop if we havent meet our desired password length
    while(!(count>= randomLen)){

        // Generate any character and add it to password c string
        int index = (rand()%(41- + 1)) + 0;
        password[count] = options[index];
        count++;
    }
    

    int passwordLength = randomLen;

    // Currently entries of same category are next to one another, so we will scramble up the c string to generate a strong password 
    for(int b=0; b< passwordLength; b++) {
        
	// Generate random index within string's range
        int randomIndex = rand() % passwordLength;
        
        // Swap password[i] with password[randomIndex];
        char temp = password[b];
        password[b] = password[randomIndex];
        password[randomIndex] = temp;
    }
    
    // Loop to print our final password
    for(int a=0; a< randomLen; a++){
        
        printf("%c", password[a]);
    }
    
    printf("\n");
    return 0;
}
