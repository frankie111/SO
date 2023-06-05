#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BLOCK_SIZE 10


int compare(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);
}
int main() {
    int pipe12[2], pipe13[2], pipe14[2], pipe25[2], pipe35[2];

    if (pipe(pipe12) < 0 || pipe(pipe13) < 0 || pipe(pipe14) < 0 || pipe(pipe25) < 0 || pipe(pipe35) < 0) {
        printf("Pipes not created..\n");
        exit(1);
    }
    char inBuffer[1000];
    int i = 2;

    printf("Write text: ");
    fgets(inBuffer, sizeof(inBuffer), stdin);
    int length = strlen(inBuffer);
    int nrBlocks = length / BLOCK_SIZE;

    for (int j = 0; j < nrBlocks; j++) {
        if (i == 2) {
            write(pipe12[1], &inBuffer[j * BLOCK_SIZE], BLOCK_SIZE);
        } else if (i == 3) {
            write(pipe13[1], &inBuffer[j * BLOCK_SIZE], BLOCK_SIZE);
        } else if (i == 4) {
            write(pipe14[1], &inBuffer[j * BLOCK_SIZE], BLOCK_SIZE);
        }
        i++;
        if (i > 4) {
            i = 2;
        }
    }
    close(pipe12[1]);
    close(pipe13[1]);
    close(pipe14[1]);
    pid_t pid2, pid3, pid4, pid5;
    //Create process 2
    if ((pid2 = fork()) < 0) {
        printf("Unable to create process\n");
        exit(1);
    } else if (pid2 == 0) {

        close(pipe13[0]);
        close(pipe14[0]);
        close(pipe35[1]);
        close(pipe12[1]);
        close(pipe13[1]);
        close(pipe14[1]);

        int sum = 0;
        char buf[1024];
        ssize_t nrBytes;

        //Read from the pipe and add the numbers to the sum variable
        while ((nrBytes = read(pipe12[0], buf, sizeof(buf))) > 0) {
            for (int i = 0; i < nrBytes; i++) {
                char c = buf[i];
                if (c >= '0' && c <= '9') {
                    sum += (c - '0');
                }
                else if (c == '\0') {
                    break;
                }
            }
        }
        write(pipe25[1], &sum, sizeof(sum));


        close(pipe12[0]);
        close(pipe25[1]);

        exit(0);
    }

    //Create process 3
    if ((pid3 = fork()) < 0) {
        printf("Unable to create process\n");
        exit(1);
    } else if (pid3 == 0) {

        //Close the read ends of the pipes
        close(pipe14[0]);
        close(pipe12[1]);
        close(pipe13[1]);
        close(pipe14[1]);
        close(pipe12[0]);
        close(pipe25[1]);
        char c;

        //Read from the pipe and convert all letters to uppercase Z
        while (read(pipe13[0], &c, 1) > 0) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                c = 'Z';
            }

            write(pipe35[1], &c, 1);
        }

        close(pipe13[0]);
        close(pipe35[1]);

        exit(0);
    }

    //Create process 4
    if ((pid4 = fork()) < 0) {
        printf("Unable to create process\n");
        exit(1);
    } else if (pid4 == 0) {

        close(pipe12[1]);
        close(pipe13[1]);
        close(pipe14[1]);
        close(pipe12[0]);
        close(pipe25[1]);
        close(pipe13[0]);
        close(pipe35[1]);
        close(pipe25[1]);
        close(pipe35[1]);
        char buffer[10000];
        int bytes_read;

        bytes_read = read(pipe14[0], buffer, 10000);

        // sort the received input using qsort and the additional function compare
        qsort(buffer, bytes_read, sizeof(char), compare);

        // print the sorted input to the console
        buffer[bytes_read]=0;
        printf("Process 4 output: %s \n", buffer);

        close(pipe14[0]);
        exit(0);
    }

    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    if((pid5= fork()) < 0) {
        printf("Unable to create process\n");
        exit(1);
    } else if (pid5==0) {

        close(pipe14[0]);
        close(pipe12[1]);
        close(pipe13[1]);
        close(pipe14[1]);
        close(pipe12[0]);
        close(pipe25[1]);
        close(pipe13[0]);
        close(pipe35[1]);
        int nr;
        char buf[10];
        printf("Sum(Process 2): ");
        if (read(pipe25[0], &nr, sizeof(nr)) > 0) {
            printf("%d\n", nr);
        }


        printf("Convert to uppercase(Process 3): ");
        while (read(pipe35[0], buf, 1) > 0) {
            printf("%s", buf);
        }

        close(pipe25[1]);
        close(pipe35[1]);
        exit(0);
    }

    close(pipe25[1]);
    close(pipe35[1]);

    //Wait for all processes to finish
    waitpid(pid4, NULL, 0);
    waitpid(pid5, NULL, 0);

    return 0;
}
