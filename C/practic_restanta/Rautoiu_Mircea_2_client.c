#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define FIFO_PATH "myfifo"

int main() {
    int fifo_fd;

    // Create the FIFO (named pipe) if it doesn't exist
    mkfifo(FIFO_PATH, 0666);

    // Get the radius from the user
    char value_str[100];
    printf("Enter radius: ");
    scanf("%99s", value_str);

    // Convert the string to a float
    char *endptr;
    errno = 0;
    float radius = strtof(value_str, &endptr);

    // Check if the conversion was successful
    if (errno != 0 || *endptr != '\0') {
        printf("Ungultige Eingabe: Non numeric!\n");
        exit(EXIT_FAILURE);
    }

    // Check if the radius is positive
    if (radius < 0) {
        printf("Ungultige Eingabe: Negatives Radius!\n");
        exit(EXIT_FAILURE);
    }

    // Open the FIFO for writing
    fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Write the radius to the FIFO
    write(fifo_fd, &radius, sizeof(radius));

    // Close the FIFO for writing
    close(fifo_fd);

    // Open the FIFO for reading
    fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Read the result from the server
    char result[100];
    read(fifo_fd, result, sizeof(result));

    // Print the result to the console
    printf("\n%s", result);

    // Close the FIFO for reading
    close(fifo_fd);
}