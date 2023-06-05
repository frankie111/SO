#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 15

// function to replace vowels with digits
void replace_vowels_with_digits(char *str) {
    for(int i = 0; str[i]; i++){
        if(str[i] == 'a' || str[i] == 'A') str[i] = '1';
        else if(str[i] == 'e' || str[i] == 'E') str[i] = '2';
        else if(str[i] == 'i' || str[i] == 'I') str[i] = '3';
        else if(str[i] == 'o' || str[i] == 'O') str[i] = '4';
        else if(str[i] == 'u' || str[i] == 'U') str[i] = '5';
    }
}

int main() {
    int fd12[2], fd13[2], fd24[2];
    pid_t pid1, pid2, pid3, pid4;

    // Create pipes
    if (pipe(fd12) < 0 || pipe(fd13) < 0 || pipe(fd24) < 0) {
        printf("Error creating pipes\n");
        exit(1);
    }

    // Create process 2
    if ((pid2 = fork()) < 0) {
        printf("Error creating process 2\n");
        exit(1);
    } else if (pid2 == 0) {
        close(fd12[1]);  // Close write end of P1-P2 pipe
        close(fd24[0]);  // Close read end of P2-P4 pipe

        char buf[MAX_LINE_LENGTH + 1];
        read(fd12[0], buf, MAX_LINE_LENGTH);
        replace_vowels_with_digits(buf);
        write(fd24[1], buf, MAX_LINE_LENGTH);

        close(fd12[0]);  // Close read end of P1-P2 pipe
        close(fd24[1]);  // Close write end of P2-P4 pipe
        exit(0);
    }

    // Create process 3
    if ((pid3 = fork()) < 0) {
        printf("Error creating process 3\n");
        exit(1);
    } else if (pid3 == 0) {
        close(fd13[1]);  // Close write end of P1-P3 pipe

        char buf[MAX_LINE_LENGTH + 1];
        read(fd13[0], buf, MAX_LINE_LENGTH);
        printf("Process 3 received: %s\n", buf);

        close(fd13[0]);  // Close read end of P1-P3 pipe
        exit(0);
    }

    // Create process 4
    if ((pid4 = fork()) < 0) {
        printf("Error creating process 4\n");
        exit(1);
    } else if (pid4 == 0) {
        close(fd24[1]);  // Close write end of P2-P4 pipe

        char buf[MAX_LINE_LENGTH + 1];
        read(fd24[0], buf, MAX_LINE_LENGTH);
        for (int i = 0; i < MAX_LINE_LENGTH; ++i) {
            if (isalnum((unsigned char)buf[i]) || buf[i] == '\n') {
                putchar(buf[i]);
            }
        }

        close(fd24[0]);  // Close read end of P2-P4 pipe
        exit(0);
    }

    // Process 1 reads from standard input and writes to the pipes
    char input_line[MAX_LINE_LENGTH + 1];
    fgets(input_line, MAX_LINE_LENGTH + 1, stdin);
    write(fd12[1], input_line, MAX_LINE_LENGTH);  // Write to P2
    write(fd13[1], input_line, MAX_LINE_LENGTH);  // Write to P3

    close(fd12[1]);  // Close write end of P1-P2 pipe
    close(fd13[1]);  // Close write end of P1-P3 pipe

    return 0;
}

