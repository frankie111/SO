#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_GRADES 10
#define MAX_BUFFER_SIZE 256

// Function to calculate the average of an array of grades
double calculate_average(int grades[], int num_grades) {
    int sum = 0;
    for (int i = 0; i < num_grades; i++) {
        sum += grades[i];
    }
    return (double)sum / num_grades;
}

int main() {
    int pipefds[2]; // An array to hold the file descriptors for the pipe
    pid_t pid; // Variable to hold the Process ID
    int grades[MAX_GRADES]; // Array to hold the grades
    int num_grades = 0; // Number of grades
    char buf[MAX_BUFFER_SIZE]; // Buffer for reading grades

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
        printf("Client Input:\n");
        printf("Enter the grades (separated by spaces, e.g., 85 90 75): ");
        fgets(buf, MAX_BUFFER_SIZE, stdin); // Read grades from standard input

        // Parse grades from the input string
        char* token = strtok(buf, " ");
        while (token != NULL && num_grades < MAX_GRADES) {
            grades[num_grades++] = atoi(token);
            token = strtok(NULL, " ");
        }

        // Write the grades to the pipe
        write(pipefds[1], &num_grades, sizeof(num_grades));
        write(pipefds[1], grades, sizeof(int) * num_grades);

        exit(EXIT_SUCCESS);
    } else { // This block will be executed by the server process
        wait(NULL); // Wait for the client process to finish

        // Read the grades from the pipe
        read(pipefds[0], &num_grades, sizeof(num_grades));
        read(pipefds[0], grades, sizeof(int) * num_grades);

        // Calculate the average grade
        double average = calculate_average(grades, num_grades);

        // Print the average grade
        printf("Server Output: The average grade is %.2f\n", average);
    }

    return 0;
}
