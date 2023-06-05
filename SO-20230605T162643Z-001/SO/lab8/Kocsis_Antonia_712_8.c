#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/wait.h>

#define FIFO_1_2 "fifo_1_2"
#define FIFO_1_3 "fifo_1_3"
#define FIFO_2_4 "fifo_2_4"
#define FIFO_3_4 "fifo_3_4"

extern int errno;

#define MAX_NUM_LINES 100
#define MAX_LINE_LENGTH 30

int main() {
    // 1. Create named pipes
    if ((mkfifo(FIFO_1_2, 0600)) && (errno != EEXIST)) {
        perror("Error creating FIFO 1_2");
        exit(1);
    }

    if (mkfifo(FIFO_1_3, 0600) && (errno != EEXIST)) {
        perror("Error creating FIFO 1_3");
        exit(1);
    }

    if (mkfifo(FIFO_2_4, 0600) && (errno != EEXIST)) {
        perror("Error creating FIFO 2_4");
        exit(1);
    }

    if (mkfifo(FIFO_3_4, 0600) && (errno != EEXIST)) {
        perror("Error creating FIFO 3_4");
        exit(1);
    }

    pid_t pid2, pid3, pid4;

    //Create process 1, that reads that from the keyboard and writes it to FIFO 1_2 and FIFO 1_3
    int fd12;
    fd12 = open(FIFO_1_2, O_RDWR);

    int fd13;
    fd13 = open(FIFO_1_3, O_RDWR);

    int fd24;
    fd24 = open(FIFO_2_4, O_RDWR);

    int fd34;
    fd34 = open(FIFO_3_4, O_RDWR);

    char inputBuffer[MAX_LINE_LENGTH];
    char lines[MAX_NUM_LINES][MAX_LINE_LENGTH + 1];
    int numLines = 0;

    printf("Enter the number of lines to be processed: ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    numLines = atoi(inputBuffer);

    if (numLines <= 0 || numLines > MAX_NUM_LINES) {
        printf("Invalid number of lines. Exiting.\n");
        return 1;
    }

    printf("Enter text to be processed:\n");
    for (int i = 0; i < numLines; i++) {
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        int bufferLength = strlen(inputBuffer);

        // Remove the newline character if it exists
        if (bufferLength > 0 && inputBuffer[bufferLength - 1] == '\n') {
            inputBuffer[bufferLength - 1] = '\0';
        }

        strncpy(lines[i], inputBuffer, MAX_LINE_LENGTH);
    }

    // Process the lines
    for (int i = 0; i < numLines; i++) {
        int lineLength = strlen(lines[i]);
        if (i % 2 == 0) {
            write(fd12, &lines[i], lineLength);
            char c = '\n';
            write(fd12, &c, 1);
        } else {
            write(fd13, &lines[i], lineLength);
            char c = '\n';
            write(fd13, &c, 1);
        }

    }
    if ((pid2 = fork()) < 0) {
        printf("Error at creating process 2\n");
        exit(1);
    } else if (pid2 == 0) {
        char buffer2[1000] = {0};

        if (read(fd12, buffer2, 1000) < 0) {
            perror("Error reading from FIFO 1_2");
            exit(1);
        }

        for (int i = 0; i < strlen(buffer2); i++) {
            if (isdigit(buffer2[i])) {
                buffer2[i] = '9';
            }
        }
        close(fd12);
        if (write(fd24, buffer2, strlen(buffer2)) < 0) {
            perror("Error writing to FIFO 2_4");
            exit(1);
        }
        exit(0);

    }
    
    if ((pid3 = fork()) < 0) {
        printf("Error creating process 3\n");
        exit(1);
    } else if (pid3 == 0) {
        char buffer3[1000];

        if (read(fd13, buffer3, 1000) < 0) {
            perror("Error reading from FIFO 1_3");
            exit(1);
        }
        for (int i = 0; i < strlen(buffer3); i++) {
            if (isalpha(buffer3[i])) {
                buffer3[i] = '0';
            }
        }
        close(fd13);
        if (write(fd34, buffer3, strlen(buffer3)) < 0) {
            perror("Error writing to FIFO 2_4");
            exit(1);
        }


        exit(0);
    }
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    if ((pid4 = fork()) < 0) {
        printf("Error creating process 4\n");
        exit(1);
    } else if (pid4 == 0) {
        printf("Process 4:\n");
        char buffer4[1000];
        char buffer3[1000];

        if (read(fd24, buffer3, 1000) < 0) {
            perror("Error reading from FIFO 2_4");
            exit(1);
        }
        close(fd24);
        printf("Process 2 output: \n");
        printf("%s", buffer3);
        printf("\n");


        if (read(fd34, buffer4, 1000) < 0) {
            perror("Error reading from FIFO 3_4");
            exit(1);
        }

        close(fd34);
        printf("Process 3 output: \n");
        printf("%s", buffer4);

        exit(0);
    }


    close(fd12);
    close(fd13);
    close(fd24);
    close(fd34);

    waitpid(pid4, NULL, 0);

    return 0;
}
