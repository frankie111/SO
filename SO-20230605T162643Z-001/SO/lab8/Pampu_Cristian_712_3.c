#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#define BUFFER_SIZE 1024

int main() {
    const char *fifo_path = "myfifo";
    char directory_path[BUFFER_SIZE];

    // Create the named pipe (FIFO)
    mkfifo(fifo_path, 0666);

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error forking process");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process (Server)
        int fifo_fd = open(fifo_path, O_RDONLY);
        if (fifo_fd == -1) {
            perror("Error opening FIFO");
            exit(EXIT_FAILURE);
        }

        read(fifo_fd, directory_path, sizeof(directory_path));

        // Check if the directory exists
        DIR *dir = opendir(directory_path);
        if (dir == NULL) {
            // Send error message to the client
            int result_fd = open(fifo_path, O_WRONLY);
            if (result_fd == -1) {
                perror("Error opening FIFO");
                exit(EXIT_FAILURE);
            }

            const char *error_message = "Error: Directory does not exist";
            write(result_fd, error_message, strlen(error_message) + 1);
            close(result_fd);

            close(fifo_fd);
            exit(EXIT_FAILURE);
        }

        closedir(dir);

        // List the contents of the directory
        DIR *directory = opendir(directory_path);
        struct dirent *entry;

        while ((entry = readdir(directory)) != NULL) {
            printf("%s\n", entry->d_name);
        }

        closedir(directory);

        close(fifo_fd);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process (Client)
        int fifo_fd = open(fifo_path, O_WRONLY);
        if (fifo_fd == -1) {
            perror("Error opening FIFO");
            exit(EXIT_FAILURE);
        }

        const char *directory_name = "."; // Use the current directory
        write(fifo_fd, directory_name, strlen(directory_name) + 1);

        close(fifo_fd);
        wait(NULL); // Wait for the child process to finish
        exit(EXIT_SUCCESS);
    }

    return 0;
}
