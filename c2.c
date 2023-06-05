#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_PATH "myfifo"

int main() {
    int fifo_fd;

    // Create the FIFO (named pipe) if it doesn't exist
    mkfifo(FIFO_PATH, 0666);

    // Open the FIFO for writing
    fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Get the kilometer count from the user
    int kilometers;
    printf("Enter kilometers: ");
    scanf("%d", &kilometers);

    // Check if the kilometer count is valid
    if (kilometers < 0) {
        printf("Invalid kilometer count\n");
        exit(EXIT_FAILURE);
    }

    // Write the kilometer count to the FIFO
    write(fifo_fd, &kilometers, sizeof(kilometers));

    // Close the FIFO for writing
    close(fifo_fd);

    // Open the FIFO for reading
    fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Read the cost from the Server
    float cost;
    read(fifo_fd, &cost, sizeof(cost));

    // Print the cost
    printf("Transport cost: %f\n", cost);

    // Close the FIFO for reading
    close(fifo_fd);

    return 0;
}