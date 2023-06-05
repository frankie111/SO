#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to calculate GCD
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int main() {
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int numbers[n];
    printf("Enter the numbers:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &numbers[i]);
    }

    pid_t pid;
    int fd[2];  // File descriptors for pipe
    pipe(fd);  // Create a pipe

    for (int i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork failed\n");
            return 1;
        } else if (pid == 0) {
            close(fd[0]);  // Close the read end of the pipe

            int result = gcd(numbers[i], numbers[i+1]);

            // Write the result to the pipe
            write(fd[1], &result, sizeof(result));
            close(fd[1]);  // Close the write end of the pipe
            exit(0);
        } else {
            wait(NULL);
            close(fd[1]);  // Close the write end of the pipe

            int child_result;
            // Read the result from the pipe
            read(fd[0], &child_result, sizeof(child_result));

            // Store the child's result in the first number of the pair
            numbers[i+1] = child_result;
        }
    }

    printf("The greatest common divisor is: %d\n", numbers[n-1]);

    return 0;
}
