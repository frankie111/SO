#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// This function will be executed by each child process
void child_process(int i, int n, char *is_prime, FILE *outfile) {
    int j;

    // Mark all multiples of i as not prime
    for (j = i*2; j <= n; j += i) {
        is_prime[j] = 0;
    }

    // Write the prime number to the output file
    fprintf(outfile, "%d\n", i);

    // Exit the child process
    exit(0);
}

int main() {
    int n;
    printf("Enter the value of N: ");
    scanf("%d", &n);

    // Create an array to keep track of which numbers are prime
    char is_prime[n+1];

    // Initialize the is_prime array to all true (1)
    int i;
    for (i = 2; i <= n; i++) {
        is_prime[i] = 1;
    }

    // Open the output file for writing
    FILE *outfile;
    outfile = fopen("primes.txt", "w");
    if (outfile == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Perform the Sieve of Eratosthenes algorithm using multiple processes
    int pid;
    for (i = 2; i <= n; i++) {
        if (is_prime[i]) {
            if ((pid = fork()) == -1) {
                // Fork error
                perror("fork error");
                exit(1);
            } else if (pid == 0) {
                // This is the child process
                child_process(i, n, is_prime, outfile);
            } else {
                // This is the parent process
                wait(NULL);

                // Mark all multiples of i as not prime
                int j;
                for (j = i*2; j <= n; j += i) {
                    is_prime[j] = 0;
                }
            }
        }
    }

    // Close the output file
    fclose(outfile);

    return 0;
}
