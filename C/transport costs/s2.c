#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_PATH "myfifo"

int main() {
    int fifo_fd;
    const float FUEL_CONSUMPTION = 5.5f; // 5.5 liters / 100 km
    const float FUEL_PRICE = 4.5f; // 4.5 lei / liter
    int kilometers;

    // Create the FIFO (named pipe) if it doesn't exist
    mkfifo(FIFO_PATH, 0666);

    // Open the FIFO for reading
    fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Read the kilometer count from the Client
    read(fifo_fd, &kilometers, sizeof(kilometers));

    // Close the FIFO for reading
    close(fifo_fd);

    // Compute the transport costs
    float fuel_consumption = (float) kilometers * FUEL_CONSUMPTION / 100;
    float transport_costs = fuel_consumption * FUEL_PRICE;

    // Open the FIFO for writing
    fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Write the transport costs to the FIFO
    write(fifo_fd, &transport_costs, sizeof(transport_costs));

    // Close the FIFO for writing
    close(fifo_fd);

    return 0;
}