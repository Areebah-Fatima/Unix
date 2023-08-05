#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int fibonacci(int n) {
   
   int prev;
   int prev2;
   int result;
   int fds[2];

   // if n is 0 or 1 return itself
   if(n == 0){
        return 0;
   }
  
   else if(n == 1) {
        return 1;
   } 

   else {
         
         // Pipe
         pipe(fds);

         // Create two children for (n-1) and (n-2)

	 // Pipe for n-1
         if (fork() == 0){
		
		// "recursive" call and write answer to pipe
                prev = fibonacci(n-1);
                write(fds[1], &prev, sizeof(int));
                exit(0);
         }

	 // Pipe for n-2
         if (fork() == 0){
                // "recursive" call and write answer to pipe     
		prev2 = fibonacci(n-2);
                write(fds[1], &prev2, sizeof(int));
                exit(0);
         }

        
         // Read results from created pipes
         read(fds[0], &prev, sizeof(int));
         read(fds[0], &prev2, sizeof(int));
         
	 // Close pipes
         close(fds[0]);
         close(fds[1]);

	 // Sequence result is (n-1) + (n-2)
         result = prev + prev2;
   
         // Return result
         return result;
   }
}

int main(int argc, char *argv[]) {
        
	int res;

	// If true it means user didn't input number so notify them
        if(argc != 2){
                printf("ERROR: Please input one number in the command line.\n");
                exit(0);
        }

        //command line input for n
        int n = atoi(argv[1]);
	
	// call helper function
        res  = fibonacci(n);
        
	// Print result and time
	printf("Final result is %d\n", res);
	//printf("Time elapsed: %f \n", time_taken);
}
