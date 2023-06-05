#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to return the partial sum of a sequence of numbers
int partial_sum(int *arr, int start, int end)
{
    int sum = 0;
    for (int i = start; i <= end; i++)
    {
        sum += arr[i];
    }
    return sum;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s n\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);

    // Generate the sequence of numbers
    int* arr = (int*) malloc(n * sizeof(int));
    printf("Enter the sequence of numbers: ");
    for (int i = 0; i < n; i++)
    {
        if (scanf("%d", &arr[i]) != 1) {
            fprintf(stderr, "Error: Failed to read integer %d from input\n", i+1);
            exit(EXIT_FAILURE);
        }
    }

    printf("Calculating partial sums for sequence [");
    for (int i = 0; i < n; i++)
    {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");

    int num_processes = 3; // the number of processes to use
    int block_size = n / num_processes; // the size of each block to process

    for (int i = 0; i < num_processes; i++)
    {
        int pid = fork();
        if (pid == -1) // fork() has failed
        {
            perror("fork() error");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) // in the child process
        {
            int start = i * block_size;
            int end = (i + 1) * block_size - 1;
            if (i == num_processes - 1)
            {
                end = n - 1;
            }
            for (int j = start; j <= end; j++)
            {
                int partial = partial_sum(arr, 0, j);
                printf("Partial sum of sequence[0..%d]: %d\n", j, partial);
            }
            exit(EXIT_SUCCESS);
        }
    }

    int status;
    for (int i = 0; i < num_processes; i++)
    {
        wait(&status);
    }

    free(arr);

    return 0;
}

