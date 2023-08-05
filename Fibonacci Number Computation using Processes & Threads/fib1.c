#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

//Sequence: 0, 1, 1, 2, 3, 5, 8, 13, 21, ...
//       n: 0  1  2  3  4  5  6   7   8  ...

// Variable to hold total number of threads
int numThreads = 0;

void *compute_fib( void *ptr )
{

	int *valuep;
	valuep = (int *) ptr; //assume that pointer is pointing to int
	int n = *valuep;

	//printf("n %d\n", n);
	
	int p1 = n-1;
	int p2 = n-2;

	//printf(" p1 %d\n", p1);
	//printf(" p2 %d\n", p2);


	
	// For when n=0 n =1 just return number itself
	if(n < 2)
	{
		return (void*) n;
	}
	
	// otherwise
	if(n>1){
	
	// ThreadID for (n-1) (n-2)
	pthread_t prev1, prev2;
	
	//create 2 threads to do computation
	
	// "recursive" n-1
	if(pthread_create(&prev1, NULL, compute_fib, (void *) &p1)){

		exit(1);
	}
	
	// Created thread so increment count
	else
	{
		numThreads++;
	}

	// "recursive" n-2
	if(pthread_create(&prev2, NULL, compute_fib, (void *) &p2)){ //&p2

	        exit(1);
	}


        // Created thread so increment count
	else
	{
		numThreads++;		

	}
	
	void *res1;
	void *res2;

	// Combine
	pthread_join(prev1,  &res1);
	pthread_join(prev2, &res2);

	// Get result
        int res = (int)res1+ (int)res2;
 	
	//printf(" res %d\n", res);
	
	// return result
	return (void *) res;	

	}
}
	

int main(int argc, char *argv[])
{

     // Thread and threadID
     pthread_t thread1;
     
     // Notify user they didnt input number to command line
     if(argc != 2){

     	printf("ERROR: Please input one number in the command line.\n");
	exit(0);
     }

     // n will hold command line input
     int n = atoi(argv[1]); 

     /* Create independent threads each of which will execute function */

     
     // create thread if it doesnt work notify user
     if (pthread_create(&thread1, NULL, compute_fib, (void *) &n)) {	// &n
	 	puts("Cannot create threads.");
		exit(1);
     }
     
     // Created thread increment count
     numThreads++;

     //printf("here");

     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

     void *rp;
     
     // Combine
     pthread_join(thread1, &rp);
     
     // Print results
     printf("\nResult: %d\n", rp);
     printf("\n# of threads: %d\n", numThreads);

     //printf("Time elapsed: %f\n ", time_taken);
     //printf("Time elapsed: %f\n ", resTime);
     
     exit(0);
}
