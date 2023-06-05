#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int N, K;
    printf("Enter the number of integers (N): ");
    scanf("%d", &N);
    printf("Enter the index of the element to find (K): ");
    scanf("%d", &K);

    int* numbers = (int*)malloc(N * sizeof(int));
    printf("Enter the integers:\n");
    for (int i = 0; i < N; i++) {
        printf("Integer %d: ", i + 1);
        scanf("%d", &numbers[i]);
    }

    // Create pipes 
    int fd_min[2];
    int fd_max[2];
    if (pipe(fd_min) == -1 || pipe(fd_max) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    // Fork a child process for determining the minimum value
    pid_t pid_min;
    pid_min = fork();

    if (pid_min == -1) {
        perror("Forking failed");
        return 1;
    }

    if (pid_min == 0) {
        // Child process for determining the minimum value
        int min = numbers[0];
        for (int i = 1; i < N; i++) {
            if (numbers[i] < min)
                min = numbers[i];
        }

        close(fd_min[0]);  
        write(fd_min[1], &min, sizeof(int));  
        close(fd_min[1]);  

        exit(0);
    } else {
        // Fork another child process for determining the maximum value
        pid_t pid_max;
        pid_max = fork();

        if (pid_max == -1) {
            perror("Forking failed");
            return 1;
        }

        if (pid_max == 0) {
            // Child process for determining the maximum value
            int max = numbers[0];
            for (int i = 1; i < N; i++) {
                if (numbers[i] > max)
                    max = numbers[i];
            }

            close(fd_max[0]);  
            write(fd_max[1], &max, sizeof(int));  
            close(fd_max[1]);  

            exit(0);
        } else {
            // Parent process
            int min, max;

            wait(NULL);  
            close(fd_min[1]); 
            read(fd_min[0], &min, sizeof(int));  
            close(fd_min[0]);  

            wait(NULL);  
            close(fd_max[1]);  

            read(fd_max[0], &max, sizeof(int));  
            close(fd_max[0]); 

            int kthElement = min;
            for (int i = 1; i < N - 1; i++) {
                if (numbers[i] > kthElement && numbers[i] < max)
                    kthElement = numbers[i];
            }

            printf("The %d-th element (when the string is sorted in ascending order) is: %d\n", K, kthElement);
        }
    }

    free(numbers);

    return 0;
}

