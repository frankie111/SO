#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>

int main() {
    int p2c[2]; // pipe parent to child
    int c2p[2]; // pipe child to parent
    int pid;

    if (pipe(p2c) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(c2p) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // child process
        // Get the filename from the user
        printf("Enter the filename: ");
        char filename[256];
        fgets(filename, sizeof(filename), stdin);

        // SO newline character from the filename
        int len = (int) strlen(filename);
        if (len > 0 && filename[len - 1] == '\n')
            filename[len - 1] = '\0';

        // Send the filename to the server
        write(c2p[1], &filename, strlen(filename) + 1);
        printf("Client: Sent filename (%s) to server.\n", filename);

        // Receive the line count from the server
        int line_count;
        read(p2c[0], &line_count, sizeof(int));
        printf("Client: Received line count (%d) from server.\n", line_count);

        close(c2p[0]);
        close(c2p[1]);
        close(p2c[0]);
        close(p2c[1]);

        exit(0);
    }

    // parent process
    // Wait for client to send the filename
    char filename[256];
    read(c2p[0], &filename, sizeof(filename));

    // Count the number of lines in the file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int line_count = 0;
    char ch;
    while ((ch = (char) fgetc(file)) != EOF)
        if (ch == '\n')
            line_count++;

    fclose(file);

    // Send the line count to the client
    write(p2c[1], &line_count, sizeof(int));
    printf("Server: Sent line count (%d) to client.\n", line_count);

    close(c2p[0]);
    close(c2p[1]);
    close(p2c[0]);
    close(p2c[1]);

    wait(0);
    return 0;
}
