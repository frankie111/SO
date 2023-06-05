#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_PATH "myfifo"

int main() {
    int fifo_fd;
    char directory_name[100];

    // Create the FIFO (named pipe) if it doesn't exist
    mkfifo(FIFO_PATH, 0666);

    // Open the FIFO for writing
    fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Get the directory name from the user
    printf("Enter directory name: ");
    fgets(directory_name, sizeof(directory_name), stdin);

    // Write the directory name to the FIFO
    write(fifo_fd, directory_name, sizeof(directory_name));

    // Close the FIFO for writing
    close(fifo_fd);

    // Open the FIFO for reading
    fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Read the directory contents from the FIFO
    char buffer[1024];
    int num_bytes = read(fifo_fd, buffer, sizeof(buffer) - 1);
    if (num_bytes == -1) {
        perror("Failed to read from FIFO");
        exit(EXIT_FAILURE);
    }
    buffer[num_bytes] = '\0'; // Null-terminate the received data

    // Print the directory contents
    printf("Directory contents:\n%s", buffer);

    // Close the FIFO for reading
    close(fifo_fd);

    return 0;
}
