#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define NUM_LETTERS 26

int main() {
    char text[] = "Aseara am cazut din pat si m am lovit cu capu de noptiera si Robi cavalerul intunecat albastru caprui";

    int count[NUM_LETTERS] = {0};
    int fd[NUM_LETTERS][2];

    // Create a pipe for each letter
    for (int i = 0; i < NUM_LETTERS; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Fork a process for each letter
    for (int i = 0; i < NUM_LETTERS; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Child process
            close(fd[i][0]);

            for (int j = 0; j < strlen(text); j++) {
                if (tolower(text[j]) == 'a' + i) {
                    count[i]++;
                }
            }

            write(fd[i][1], &count[i], sizeof(count[i]));
            close(fd[i][1]);
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process
    for (int i = 0; i < NUM_LETTERS; i++) {
        close(fd[i][1]);
        read(fd[i][0], &count[i], sizeof(count[i]));
        close(fd[i][0]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < NUM_LETTERS; i++) {
        wait(NULL);
    }

    // Print results
    for (int i = 0; i < NUM_LETTERS; i++) {
        printf("%c: %d\n", 'A' + i, count[i]);
    }

    return 0;
}

