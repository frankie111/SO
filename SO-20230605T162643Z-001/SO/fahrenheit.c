#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 256

// Convert Celsius to Fahrenheit
double celsius_to_fahrenheit(double celsius) {
    return celsius * 9.0 / 5.0 + 32;
}

// Convert Fahrenheit to Celsius
double fahrenheit_to_celsius(double fahrenheit) {
    return (fahrenheit - 32) * 5.0 / 9.0;
}

int main() {
    int pipefds[2]; // An array to hold the file descriptors for the pipe
    pid_t pid; // Variable to hold the Process ID
    char buf[MAX_BUFFER_SIZE]; // Buffer for reading the temperature
    char scale[MAX_BUFFER_SIZE]; // Buffer for reading the scale
    double temperature, converted;

    // Create the pipe
    if (pipe(pipefds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Create the child process
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // This block will be executed by the client process
    if (pid == 0) {
        printf("Client Input: \n");
        printf("Temperature Value: ");
        fgets(buf, MAX_BUFFER_SIZE, stdin); // Read temperature from standard input
        temperature = atof(buf); // Convert string to double

        printf("Scale (Celsius or Fahrenheit): ");
        fgets(scale, MAX_BUFFER_SIZE, stdin); // Read scale from standard input
        // fgets() includes the newline in the string, so we need to remove it
        // strcspn() returns the index of the first occurrence of any character in the second argument
        // If no character from the second argument is found, it returns the length of the string
        // Therefore, this line replaces the newline with a null terminator
        scale[strcspn(scale, "\n")] = 0;

        // Write the temperature and scale to the pipe
        write(pipefds[1], &temperature, sizeof(temperature));
        write(pipefds[1], scale, sizeof(scale));
        exit(EXIT_SUCCESS);
    } else { // This block will be executed by the server process
        wait(NULL); // Wait for the client process to finish

        // Read the temperature and scale from the pipe
        read(pipefds[0], &temperature, sizeof(temperature));
        read(pipefds[0], scale, sizeof(scale));

        // Convert the temperature
        if (strcmp(scale, "Celsius") == 0) {
            converted = celsius_to_fahrenheit(temperature);
        } else if (strcmp(scale, "Fahrenheit") == 0) {
            converted = fahrenheit_to_celsius(temperature);
        } else {
            printf("Invalid Scale\n");
            exit(EXIT_FAILURE);
        }

        // Print the converted temperature
        printf("Server Output: %.2f\n", converted);
    }

    return 0;
}

//Write a client-server program that converts temperatures between Celsius and Fahrenheit scales. The client sends a temperature value and the desired scale (Celsius or Fahrenheit) to the server. The server performs the conversion and returns the converted temperature value to the client. Make sure you handle exceptions in your solution.
//Useful formula:
//
//Conversion from Celsius to Fahrenheit:
//Fahrenheit = Celsius * 9/5 + 32. - Conversion from Fahrenheit to Celsius:
//Celsius = (Fahrenheit - 32) * 5/9
//Additional requirements:
//Handle exceptions for invalid inputs, such as non-numeric values for the temperature.
//Handle exceptions for invalid scale values and only accept "Celsius" or "Fahrenheit".
//Example 1:
//input Client:
//Temperature value: 25
//Scale: Celsius
//        Example 2:
//input Client:
//Temperature value:
//twenty-five
//        Scale: Celsius
//        Example 3:
//input Client:
//Temperature value: 25
//Scale: Kelvin
//        output Server:
//77
//output Client:
//Received from Server: 77 Fahrenheit
//        output Server:
//Invalid input
//output Client:
//Please enter a valid value.
//output Server:
//Invalid input
//output Client:
//Please enter a valid value.