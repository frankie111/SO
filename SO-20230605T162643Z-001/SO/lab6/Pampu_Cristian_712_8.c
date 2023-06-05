#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>

#define N 5  // the number of elements in the sequence

int main() {
    // define the sequence
    int sequence[N] = {1, 2, 3, 4, 5};

    // allocate shared memory to hold the partial sums
    int *partial_sums = mmap(NULL, N * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    for (int i = 0; i < N; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            // fork failed
            perror("fork");
            return 1;
        } else if (pid == 0) {
            // child process
            if (i == 0) {
                partial_sums[i] = sequence[i];
            } else {
                partial_sums[i] = partial_sums[i - 1] + sequence[i];
            }
            exit(0);  // end the child process
        }
    }

    // wait for all children to finish
    for (int i = 0; i < N; ++i) {
        wait(NULL);
    }

    // print the partial sums
    for (int i = 0; i < N; ++i) {
        printf("Partial sum of first %d elements: %d\n", i + 1, partial_sums[i]);
    }

    // free the shared memory
    munmap(partial_sums, N * sizeof(int));

    return 0;
}
