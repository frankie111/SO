#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1
#define BUFFER_SIZE 256
#define INVALID_INPUT -1.0

// Function to calculate discounted price
float calculate_discount(float original_price, float discount) {
    return original_price - (original_price * (discount / 100.0));
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
        float original_price, discount;
        char *endptr;

        // Get original price from the user
        printf("Original price: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        // Convert the string to float using strtof.
        // strtof returns the converted float value and sets endptr to point to any remaining characters after the float.
        // If the conversion fails (e.g., if the input is not a number), strtof returns 0.0 and sets endptr to the start of the input string.
        original_price = strtof(buffer, &endptr);

        // Get discount percentage from the user
        printf("Discount percentage: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        // Similar conversion process as for the original price
        discount = strtof(buffer, &endptr);

        // Write original price and discount to the write end of pipe fd1
        write(fd1[WRITE], &original_price, sizeof(original_price));
        write(fd1[WRITE], &discount, sizeof(discount));

        // Read discounted price from the read end of pipe fd2
        float discounted_price;
        int status_code;
        read(fd2[READ], &discounted_price, sizeof(discounted_price));
        read(fd2[READ], &status_code, sizeof(status_code));

        // Check the status code
        if(status_code == INVALID_INPUT) {
            printf("Client: Received from server: Please enter a value between 0.00 and 100.00.\n");
        } else {
            printf("Client: Received from server: %.2f\n", discounted_price);
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
        read(fd1[READ], &discount, sizeof(discount));

        // Check if original price and discount are valid
        if(original_price <= 0 || discount < 0.0 || discount > 100.0) {
            printf("Server: Invalid input\n");
            float invalid = INVALID_INPUT;
            // If the input is invalid, set the status code to INVALID_INPUT
            status_code = INVALID_INPUT;
            write(fd2[WRITE], &invalid, sizeof(invalid));
            write(fd2[WRITE], &status_code, sizeof(status_code));
            exit(1);
        }

        // Calculate discounted price and print it
        float discounted_price = calculate_discount(original_price, discount);
        printf("Server: Final price after applying discount: %.2f\n", discounted_price);

        // Write discounted price to the write end of pipe fd2
        write(fd2[WRITE], &discounted_price, sizeof(discounted_price));
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


//Write a client-server program that calculates the discounted price for a specific product. The client sends the original price and the discount percentage to the server. The server returns the final price after applying the discount. The client displays the received result. Make sure to handle exceptions in your solution.
//Additional requirements:
//
//The discount percentage should be a positive floating-point number between 0.00 and 100.00.
//Handle exceptions for invalid inputs, such as non-numeric values or discount percentages outside the valid range.
//Example 1:
//input Client:
//Original price: 100.00
//Discount percentage: 20.00
//output Server:
//Final price after applying the discount: 80.00
//output Client:
//Received from the server: 80.00

//Example 2:
//input Client:
//Original price: 100.00
//Discount percentage: 150.00

//Example 3:
//input Client:
//Original price: ten
//        Discount percentage: 150.00
//output Server:
//Invalid input
//output Client:
//Received from the server: Please enter a value between 0.00 and 100.00.
//output Server:
//Invalid input
//output Client:
//Received from the server: Please enter a value between 0.00 and 100.00.