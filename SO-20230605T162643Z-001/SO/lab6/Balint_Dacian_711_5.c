#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Insufficient arguments");
        return 1;
    }

    int n = atoi(argv[1]);
    int* numbers = (int*) malloc(n * sizeof(int));

    if (argc < n + 2) {
        printf("Error: insufficient number of arguments\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        numbers[i] = atoi(argv[i + 2]);
    }

    int result = 0;

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            printf("Error: fork failed.\n");
            exit(1);
        } else if (pid == 0) {
            // Child process
            int gcd_res = gcd(numbers[i], result);
            exit(gcd_res);
        }
	
    }

    // Parent process
    int status;
    for (int i = 0; i < n; i++) {
        wait(&status);
        int gcd_res = WEXITSTATUS(status);
        result = gcd(result, gcd_res);
    }

    printf("The greatest common divisor is: %d\n", result);

    free(numbers);
    return 0;
}

