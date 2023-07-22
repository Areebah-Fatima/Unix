#include<stdio.h>
#include<string.h>
#include <ctype.h>

int main()
{
    int const size = 150;
    
    // Create c strings to hold user input
    char str[size];
    char str2[size];
    
    // Flag to see if strings match
    int same = 1;

    // Prompt user for input and store it
    printf("Enter String #1: \n");
    fgets(str, 150, stdin);

    // Prompt user for input and store it
    printf("Enter String #2: \n");
    fgets(str2, 150, stdin);
    
    // Need 2 indexes to deal with whitespaces
    int arr1Ind = 0;
    int arr2Ind = 0;

    // Walk array and make both strings lower case
    for(int i=0; i<150; i++) 
    {

        str[i] = tolower(str[i]);
        str2[i] = tolower(str2[i]);

    }
    

    // Stop when you reach the null terminator for the string
    while(str[arr1Ind]!='\0'||str2[arr2Ind]!='\0') 
    { 
        
        // Find white spaces and ignore them
        if(str[arr1Ind]==' '||str2[arr2Ind]==' ') 
        { 
            // Deal with white space depending on where they show up
            
            if(str[arr1Ind]==' ') 
            {
                
                arr1Ind++;
                
            }
            if(str2[arr2Ind] == ' ') 
            {
                
                arr2Ind++;
                
            }
        }
        
        // If character dont = means strings are different so change our flag
        else if(str[arr1Ind] != str2[arr2Ind]) 
        { 
            
            same = 0;
            
            // Dont need to search anymore
            break;
        }
        
        // Character at that index is the same
        if(str[arr1Ind] == str2[arr2Ind]) 
        {

            // Move on to next index
            arr1Ind++;
            arr2Ind++;

        }
    }
    
    while(str[arr1Ind] != '\0')
    {
        // Since one string has already ended the only way to match is if the other string only has whitespaces
        
        // Run into a non whitespace character means string cant match
        if(str[arr1Ind] != ' ')
        {
            
            // Set flag
            same = 0;
            break;
            
        }
        
        // Just whitespace increment counter
        else
        {
            arr1Ind++;
            
        }
    }
  
     while(str2[arr2Ind] != '\0')
     {
        
        // Run into a non whitespace character means string cant match
        if(str2[arr2Ind] != ' ')
        {
            same = 0;
            break;
        }
        
        // Just whitespace increment counter
        else
        {
            
            arr2Ind++;
            
        }
    }
    
    // Flag = one means different string 1 means same (print result)
    if(same == 1)
    {
        
        printf("Both strings are same!\n");
        
    }
    
    else
    {
        
        printf("Both strings are not same!\n");
        
    }
    
    return 0;
}
