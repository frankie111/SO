#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    int pipefd[2];
    pid_t childpid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    childpid = fork();

    if (childpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (childpid == 0) {
        // Child process (server)
        close(pipefd[1]);  // Close the write end of the pipe

        // Read the directory name from the pipe
        char directory[BUFFER_SIZE];
        ssize_t bytesRead = read(pipefd[0], directory, sizeof(directory) - 1);
        if (bytesRead > 0) {
            directory[bytesRead] = '\0';
            printf("Received directory name from client: %s\n", directory);

            DIR *dir = opendir(directory);
            if (dir != NULL) {
                // Directory exists
                struct dirent *entry;
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_type == DT_REG) {
                        printf("%s\n", entry->d_name);
                    }
                }
                closedir(dir);
            } else {
                // Directory doesn't exist
                printf("Directory '%s' does not exist.\n", directory);
            }
        }

        close(pipefd[0]);  // Close the read end of the pipe
        exit(EXIT_SUCCESS);
    } else {
        // Parent process (client)
        close(pipefd[0]);  // Close the read end of the pipe

        // Write the directory name to the pipe
        char *directory = argv[1];
        write(pipefd[1], directory, strlen(directory));

        close(pipefd[1]);  // Close the write end of the pipe

        wait(NULL);  // Wait for the child process to complete

        exit(EXIT_SUCCESS);
    }
}
