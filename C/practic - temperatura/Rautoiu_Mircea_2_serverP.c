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

    // Read the temperature value from the client
    float value;
    read(fifo_fd, &value, sizeof(value));

    // Read the unit from the client
    char unit[100];
    int num_bytes = read(fifo_fd, unit, sizeof(unit));
    if (num_bytes == -1) {
        perror("Failed to read from FIFO");
        exit(EXIT_FAILURE);
    }

    // Open the FIFO for writing
    fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    float temp;
    char converted[100];
    // Convert the temperature and send it back to the client as a string
    if (strcmp(unit, "Celsius") == 0) { // Convert to Fahrenheit
        temp = (float) value * 9 / 5 + 32;
        // Concatenate temp and unit into a string
        sprintf(converted, "%f %s", temp, "Fahrenheit");
    } else {
        temp = (float) (value - 32) * 5 / 9;
        sprintf(converted, "%f %s", temp, "Celsius");
    }

    // Write the converted temperature to the console
    printf("%s\n", converted);

    // Write the converted temperature to the FIFO
    write(fifo_fd, converted, sizeof(converted));

    // Close the FIFO for writing
    close(fifo_fd);

}