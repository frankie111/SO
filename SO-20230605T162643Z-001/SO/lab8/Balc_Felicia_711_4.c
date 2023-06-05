#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#define MAX_FILENAME_LENGTH 256
#define MAX_BUFFER_SIZE 1024


int countWords(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int count = 0;
    int inWord = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (isalpha(c)) {
            if (inWord == 0) {
                inWord = 1;
                count++;
            }
        } else {
            inWord = 0;
        }
    }

    fclose(file);
    return count;
}

int main() {
    pid_t pid;
    int pipe_fd;
    char filename[MAX_FILENAME_LENGTH];

// Remove the named pipe file if it exists
    unlink("fifo_pipe");

    // Create the named pipe
    if (mkfifo("fifo_pipe", 0666) == -1) {
        perror("Error creating named pipe");
        exit(EXIT_FAILURE);
    }
 

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("Error forking process");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process (receiver)

        // Open the named pipe for reading
        pipe_fd = open("fifo_pipe", O_RDONLY);
        if (pipe_fd == -1) {
            perror("Error opening named pipe for reading");
            exit(EXIT_FAILURE);
        }

        // Read the filename from the named pipe
        if (read(pipe_fd, filename, MAX_FILENAME_LENGTH) == -1) {
            perror("Error reading from named pipe");
            exit(EXIT_FAILURE);
        }

        close(pipe_fd);

        // Calculate the number of words
        int num_words = countWords(filename);

        // Open the named pipe for writing
        pipe_fd = open("fifo_pipe", O_WRONLY);
        if (pipe_fd == -1) {
            perror("Error opening named pipe for writing");
            exit(EXIT_FAILURE);
        }

        // Write the number of words to the named pipe
        if (write(pipe_fd, &num_words, sizeof(int)) == -1) {
            perror("Error writing to named pipe");
            exit(EXIT_FAILURE);
        }

        close(pipe_fd);

        exit(EXIT_SUCCESS);
    } else {
        // Parent process (sender)

        // Read the filename from the user
        printf("Enter the filename: ");
        fgets(filename, MAX_FILENAME_LENGTH, stdin);
        filename[strcspn(filename, "\n")] = '\0';  

        // Open the named pipe for writing
        pipe_fd = open("fifo_pipe", O_WRONLY);
        if (pipe_fd == -1) {
            perror("Error opening named pipe for writing");
            exit(EXIT_FAILURE);
        }

        // Write the filename to the named pipe
        if (write(pipe_fd, filename, strlen(filename)) == -1) {
            perror("Error writing to named pipe");
            exit(EXIT_FAILURE);
        }

        close(pipe_fd);

        // Open the named pipe for reading
        pipe_fd = open("fifo_pipe", O_RDONLY);
        if (pipe_fd == -1) {
            perror("Error opening named pipe for reading");
	    exit(EXIT_FAILURE);
        }
	        // Read the number of words from the named pipe
        int num_words;
        if (read(pipe_fd, &num_words, sizeof(int)) == -1) {
            perror("Error reading from named pipe");
            exit(EXIT_FAILURE);
        }

        close(pipe_fd);

        // Display the result
        printf("Number of words in the file: %d\n", num_words);

        // Remove the named pipe
        if (unlink("fifo_pipe") == -1) {
            perror("Error removing named pipe");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }

    return 0;
}

