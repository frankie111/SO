#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main() {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process (Server)
        close(pipefd[WRITE_END]);

        // Wait for client to send the filename
        char filename[256];
        read(pipefd[READ_END], filename, sizeof(filename));

        // Count the number of lines in the file
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        int line_count = 0;
        char ch;
        while ((ch = fgetc(file)) != EOF) {
            if (ch == '\n') {
                line_count++;
            }
        }
        fclose(file);

        // Send the line count to the client
        close(pipefd[READ_END]);
        write(pipefd[WRITE_END], &line_count, sizeof(line_count));
        printf("Server: Sent line count (%d) to client.\n", line_count);

        // Wait for the client process to exit
        wait(NULL);
    } else {
        // Child process (Client)
        close(pipefd[READ_END]);

        // Get the filename from the user
        printf("Enter the filename: ");
        char filename[256];
        fgets(filename, sizeof(filename), stdin);

        // SO newline character from the filename
        int len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n') {
            filename[len - 1] = '\0';
        }

        // Send the filename to the server
        write(pipefd[WRITE_END], filename, strlen(filename) + 1);
        printf("Client: Sent filename (%s) to server.\n", filename);

        // Receive the line count from the server
        int line_count;
        read(pipefd[READ_END], &line_count, sizeof(int));
        printf("Client: Received line count (%d) from server.\n", line_count);

        close(pipefd[WRITE_END]);
    }

    return 0;
}
