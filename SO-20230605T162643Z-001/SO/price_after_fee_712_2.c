#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1
#define BUFFER_SIZE 256
#define INVALID_INPUT -1.0

// Function to calculate discounted price
float calculate_fee(float original_price) {
    return original_price + (original_price * (10 / 100.0));
}

int main() {
    // Create two pipes, fd1 and fd2
    int fd1[2], fd2[2];

    // Initialize the pipes
    pipe(fd1);
    pipe(fd2);

    // Create a child process
    pid_t pid = fork();

    if (pid > 0) {
        // Parent process: Client
        // Close unnecessary ends of the pipes
        close(fd1[READ]);
        close(fd2[WRITE]);

        char buffer[BUFFER_SIZE];
        float original_price;
        char *endptr;

        // Get original price from the user
        printf("Betrag auf der Rechnung: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        // Convert the string to float using strtof.
        // strtof returns the converted float value and sets endptr to point to any remaining characters after the float.
        // If the conversion fails (e.g., if the input is not a number), strtof returns 0.0 and sets endptr to the start of the input string.
        original_price = strtof(buffer, &endptr);

        // Write original price and discount to the write end of pipe fd1
        write(fd1[WRITE], &original_price, sizeof(original_price));

        // Read discounted price from the read end of pipe fd2
        float price_after_fee;
        int status_code;
        read(fd2[READ], &price_after_fee, sizeof(price_after_fee));
        read(fd2[READ], &status_code, sizeof(status_code));

        // Check the status code
        if(status_code == INVALID_INPUT) {
            printf("Client: Empfang vom Server: Bitte geben Sie einen gültigen Wert.\n");
        } else {
            printf("Client: Empfang vom Server: %.2f\n", price_after_fee);
        }

        // Close the remaining ends of the pipes
        close(fd1[WRITE]);
        close(fd2[READ]);
    } else if (pid == 0) {
        // Child process: Server
        // Close unnecessary ends of the pipes
        close(fd1[WRITE]);
        close(fd2[READ]);

        float original_price, discount;
        int status_code = 0; // Initialize status code as 0 (no error)

        // Read original price and discount from the read end of pipe fd1
        read(fd1[READ], &original_price, sizeof(original_price));

        // Check if original price and discount are valid
        if(original_price <= 0) {
            printf("Server: Ungültige Eingabe\n");
            float invalid = INVALID_INPUT;
            // If the input is invalid, set the status code to INVALID_INPUT
            status_code = INVALID_INPUT;
            write(fd2[WRITE], &invalid, sizeof(invalid));
            write(fd2[WRITE], &status_code, sizeof(status_code));
            exit(1);
        }

        // Calculate discounted price and print it
        float price_after_fee = calculate_fee(original_price);
        printf("Server: Preis mit Servicegebühr: %.2f\n", price_after_fee);

        // Write discounted price to the write end of pipe fd2
        write(fd2[WRITE], &price_after_fee, sizeof(price_after_fee));
        write(fd2[WRITE], &status_code, sizeof(status_code));

        // Close the remaining ends of the pipes
        close(fd1[READ]);
        close(fd2[WRITE]);
    } else {
        // Fork failed
        printf("Fork failed\n");
        return 1;
    }

    return 0;
}
