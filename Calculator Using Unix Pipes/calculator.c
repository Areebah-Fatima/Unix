#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Constant for max length
#define MAXLEN 1000

int fds[100][2];
char operators[100];
int operatorCount = 0;
int numPipe = 0;

void child(int index) {

        //rewrite the pipes for fds 0, 1 & 3 before invoking add/subtract/multiply/divide
        //setup FD 0 for reading 1st parameter
        close(0);

        if(!index){

                dup(fds[0][0]);
        }

        else{

                dup(fds[operatorCount+index][0]);

        }

        //setup FD 3 for reading 2nd parameter
        close(3);
        dup(fds[index+1][0]);
        //setup FD 1 for outputing the result
        close(1);
        dup(fds[operatorCount+index+1][1]);

        for(int i=0; i< numPipe; i++){

                close(fds[i][0]);
                close(fds[i][1]);

        }

        switch(operators[index]){

                case '+':
                        execl("./add", "add", NULL);

                case '-':
                        execl("./subtract", "subtract", NULL);

                case '/':
                        execl("./divide", "divide", NULL);

                case '*':
                        execl("./multiply", "multiply", NULL);
        }

        fputs("I hope you do not see me!", stderr);
        exit(1);
}


int main(int argc, char *argv[]) {

        char line[MAXLEN], *temp;

        if(argc < 2){
                fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(1);
        }

        // open file
        FILE *dataFile = fopen(argv[1], "r");

        //read the first line - it contains the configuration
        fgets(line, MAXLEN, dataFile);

        // sample content for line: a + b - c
        strtok(line, " \n"); //skip the symbol representing variable/parameter

        // int operatorCount=0;
        while (temp = strtok(NULL, " \n")) {
                operators[operatorCount] = temp[0];
               // printf("operator: %c\n", operators[operatorCount]);
                operatorCount++;
                strtok(NULL, " \n"); //skip the symbol representing variable/parameter
        }

        //setup the configuration with necessary # of children
        numPipe = operatorCount*2+1;

        for(int i = 0; i<numPipe; i++){

                pipe(fds[i]);
        }

        // create children for each operator
        for(int i = 0; i<operatorCount; i++){

                if(!fork()){

                        //child(i, operators[i]);
                        child(i);
                }
        }

        //continue to read the data from the file
        //you can use fscanf() to read the remaining data!
        //Here is some code to get started!

        int x;
        while (fscanf(dataFile, "%d", &x) > 0) {

                write(fds[0][1], &x, sizeof(int));

                // write to appropriate pipe
                for(int i=1; i<=operatorCount; i++){
                        fscanf(dataFile, "%d", &x);
                        write(fds[i][1], &x, sizeof(int));
                }

                // display final output that is read
                read(fds[2*operatorCount][0], &x, sizeof(int));
                printf("%d\n", x);
        }

}
