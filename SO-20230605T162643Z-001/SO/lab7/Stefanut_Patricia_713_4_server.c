#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 256

int countWords(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int wordCount = 0;
    char buffer[BUFFER_SIZE];

    while (fscanf(file, "%s", buffer) != EOF) {
        wordCount++;
    }

    fclose(file);

    return wordCount;
}

int main() {
    const char *fifoFile = "/tmp/wordcount_fifo";
    int fifoFd;
    ssize_t bytesRead, bytesWritten;
    char filename[BUFFER_SIZE];
    char wordCountStr[BUFFER_SIZE];

    // Create the FIFO if it doesn't exist
    if (mkfifo(fifoFile, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Open the FIFO for reading
        fifoFd = open(fifoFile, O_RDONLY);
        if (fifoFd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Read the filename from the FIFO
        bytesRead = read(fifoFd, filename, sizeof(filename) - 1);
        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        filename[bytesRead] = '\0';

        // Close the FIFO
        close(fifoFd);

        // Get the word count
        int wordCount = countWords(filename);

        // Convert word count to string
        snprintf(wordCountStr, BUFFER_SIZE, "%d", wordCount);

        // Open the FIFO for writing the result
        fifoFd = open(fifoFile, O_WRONLY);
        if (fifoFd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Write the word count to the FIFO
        bytesWritten = write(fifoFd, wordCountStr, strlen(wordCountStr) + 1);
        if (bytesWritten == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        // Close the FIFO
        close(fifoFd);
    }

    // Remove the FIFO
    if (unlink(fifoFile) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
