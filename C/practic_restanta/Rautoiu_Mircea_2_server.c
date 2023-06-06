#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define FIFO_PATH "myfifo"

int main() {
    int fifo_fd;

    // Create the FIFO (named pipe) if it doesn't exist
    mkfifo(FIFO_PATH, 0666);

    // Open the FIFO for reading
    fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Read the radius from the fifo
    float radius;
    read(fifo_fd, &radius, sizeof(radius));

    // Compute length and area
    float PI = 3.14f;
    float length = 2 * PI * radius;
    float area = PI * radius * radius;

    // Close the FIFO for reading
    close(fifo_fd);

    // Create result string
    char result[100];
    sprintf(result, "Lange: %f \nFlacheninhalt: %f", length, area);

    // Write the result to the console
    printf("%s\n", result);

    // Open the FIFO for writing
    fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Write the result to the FIFO
    write(fifo_fd, result, sizeof(result));

    // Close the FIFO for writing
    close(fifo_fd);
}