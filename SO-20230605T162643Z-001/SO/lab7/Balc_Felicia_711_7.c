#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

int main() {
    int pipe1to2[2]; // Pipe for communication between process 1 and process 2
    int pipe1to3[2]; // Pipe for communication between process 1 and process 3
    int pipe2to4[2]; // Pipe for communication between process 2 and process 4

    // Create the pipes
    pipe(pipe1to2);
    pipe(pipe1to3);
    pipe(pipe2to4);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child process 1
        close(pipe1to2[0]); // Close the reading end of pipe1to2
        close(pipe1to3[0]); // Close the reading end of pipe1to3
        close(pipe2to4[0]); // Close the reading end of pipe2to4

        // Process 1 logic

        char line[256];
        ssize_t bytesRead;

        while ((bytesRead = read(STDIN_FILENO, line, sizeof(line) - 1)) > 0) {
            line[bytesRead] = '\0';

            if (strlen(line) <= 15) {
                write(pipe1to2[1], line, strlen(line));
                write(pipe1to2[1], "\n", 1);
		write(pipe1to3[1], line, strlen(line));
                write(pipe1to3[1], "\n", 1);
            }
        }

        close(pipe1to2[1]); 
        close(pipe1to3[1]);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child process 2
        close(pipe1to3[0]); // Close the reading end of pipe1to3
        close(pipe2to4[0]); // Close the reading end of pipe2to4
        close(pipe1to3[1]); // Close the writing end of pipe1to3

        // Process 2 logic
        char buffer[256];
        ssize_t bytesRead;

        while ((bytesRead = read(pipe1to2[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';

            
	    for (ssize_t i = 0; i < bytesRead; i++) {
        	if (tolower(buffer[i]) == 'a')
            	    buffer[i] = '1';
        	else if (tolower(buffer[i]) == 'e')
            	    buffer[i] = '2';
        	else if (tolower(buffer[i]) == 'i')
            	    buffer[i] = '3';
        	else if (tolower(buffer[i]) == 'o')
            	    buffer[i] = '4';
        	else if (tolower(buffer[i]) == 'u')
            	    buffer[i] = '5';
    	    }
            write(pipe2to4[1], buffer, bytesRead);
        }

        close(pipe1to2[0]); // Close the reading end of pipe1to2
        close(pipe2to4[1]); // Close the writing end of pipe2to4

        exit(0);
    }


    pid_t pid3 = fork();
    if (pid3 == 0) {
        // Child process 3
        close(pipe1to2[0]); // Close the reading end of pipe1to2
        close(pipe1to2[1]); // Close the writing end of pipe1to2
        close(pipe2to4[0]); // Close the reading end of pipe2to4
        close(pipe2to4[1]); // Close the writing end of pipe2to4
        
	// Process 3 logic
        
	char buffer[256];
        ssize_t bytesRead;

        while ((bytesRead = read(pipe1to3[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
	    printf("Process 3 output:\n"); 
            printf("%s", buffer);
        }

        close(pipe1to3[0]); // Close the reading end of pipe1to3
        close(pipe1to3[1]); // Close the writing end of pipe1to3
        exit(0);
    }

    pid_t pid4 = fork();
    if (pid4 == 0) {
        // Child process 4
        close(pipe1to2[0]); // Close the reading end of pipe1to2
        close(pipe1to2[1]); // Close the writing end of pipe1to2
        close(pipe1to3[0]); // Close the reading end of pipe1to3
        close(pipe1to3[1]); // Close the writing end of pipe1to3

        // Process 4 logic

        char buffer[256];
        ssize_t bytesRead;

        while ((bytesRead = read(pipe2to4[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
	    int hasLetter = 0;
    	    int hasNumber = 0;

            for (ssize_t i = 0; i < bytesRead; i++) {
        	if (isalpha(buffer[i])) {
                    hasLetter = 1;
                } else if (isdigit(buffer[i])) {
                    hasNumber = 1;
                }

                if (hasLetter && hasNumber) {
                    printf("Process 4 output:\n"); 
                    printf("%s", buffer);
                    break;
        	}
    	    }
        }

        close(pipe2to4[0]); // Close the reading end of pipe2to4
        close(pipe2to4[1]); // Close the writing end of pipe2to4
        exit(0);
    }

    // Close all remaining pipe ends in the parent process
    close(pipe1to2[0]);
    close(pipe1to2[1]);
    close(pipe1to3[0]);
    close(pipe1to3[1]);
    close(pipe2to4[0]);
    close(pipe2to4[1]);

    // Wait for all child processes to finish
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}
