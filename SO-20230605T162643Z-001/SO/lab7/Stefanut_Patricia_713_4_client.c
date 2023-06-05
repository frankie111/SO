#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *fifoFile = "/tmp/wordcount_fifo";
    int fifoFd;
    ssize_t bytesWritten, bytesRead;
    char buffer[BUFFER_SIZE];

    // Open the FIFO for writing
    fifoFd = open(fifoFile, O_WRONLY);
    if (fifoFd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write the filename to the FIFO
    bytesWritten = write(fifoFd, argv[1], strlen(argv[1]) + 1);
    if (bytesWritten == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Close the FIFO
    close(fifoFd);

    // Open the FIFO for reading the result
    fifoFd = open(fifoFile, O_RDONLY);
    if (fifoFd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read the word count from the FIFO
    bytesRead = read(fifoFd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    buffer[bytesRead] = '\0';

    // Close the FIFO
    close(fifoFd);

    // Print the word count
    printf("Anzahl der Worter in '%s': %s\n", argv[1], buffer);

    exit(EXIT_SUCCESS);
}

