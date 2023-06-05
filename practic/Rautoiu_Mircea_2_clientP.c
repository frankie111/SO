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

    // Get the value from the user using strtof
    char value_str[100];
    printf("Enter temperature value: ");
    scanf("%99s", value_str);

    // Convert the string to float using strtof
    char *endptr;
    errno = 0;
    float value = strtof(value_str, &endptr);

    // Check if the conversion was successful
    if (errno != 0 || *endptr != '\0') {
        printf("Invalid value\n");
        exit(EXIT_FAILURE);
    }

    // Ignore the newline character
    getchar();

    // Get the unit from the user
    char unit[100];
    printf("Enter the unit: ");
    scanf("%99s", unit);

    // Check if the unit is valid
    if (strcmp(unit, "Celsius") != 0 && strcmp(unit, "Fahrenheit") != 0) {
        printf("Invalid Unit");
        exit(EXIT_FAILURE);
    }

    // Open the FIFO for writing
    fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Write the temperature to the FIFO
    write(fifo_fd, &value, sizeof(value));

    // Write the unit to the FIFO
    write(fifo_fd, unit, sizeof(unit));

    // Close the FIFO for writing
    close(fifo_fd);

    // Open the FIFO for reading
    fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Read the result from the server: the converted temperature and unit
    char converted[100];
    read(fifo_fd, converted, sizeof(converted));

    // Close the FIFO for reading
    close(fifo_fd);

    // Print the converted temperature and unit
    printf("\nFrom server: %s", converted);
}