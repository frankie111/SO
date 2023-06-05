#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

void printTxtFilesRecursive(const char* directory) {
    DIR* dir = opendir(directory);

    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".txt") != NULL) {
            printf("%s/%s\n", directory, entry->d_name);
        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char path[BUFFER_SIZE];
            snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
            printTxtFilesRecursive(path);
        }
    }

    closedir(dir);
}

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process (server)
        close(pipefd[1]); // Close the write end of the pipe

        char dirname[BUFFER_SIZE];
        read(pipefd[0], dirname, sizeof(dirname));
        close(pipefd[0]);

        printTxtFilesRecursive(dirname);

    } else {
        // Parent process (client)
        close(pipefd[0]); // Close the read end of the pipe

        char dirname[BUFFER_SIZE];
        printf("Enter directory name: ");
        fgets(dirname, sizeof(dirname), stdin);
        dirname[strcspn(dirname, "\n")] = '\0'; // Remove trailing newline

        write(pipefd[1], dirname, strlen(dirname) + 1);
        close(pipefd[1]);

        wait(NULL); // Wait for the child process to finish
    }

    return 0;
}

