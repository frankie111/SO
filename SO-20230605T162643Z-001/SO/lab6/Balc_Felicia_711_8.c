#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    int n = sizeof(nums) / sizeof(nums[0]);
    int i, subtotal = 0;
    int p = 0;

    for (i = 0; i < n; i++) {
        p = fork();

        if (p == -1) {
            perror("fork error");
            exit(EXIT_FAILURE);
        }

        if (p == 0) {  // Child process
            int j;
            for (j = 0; j <= i; j++) {
                subtotal += nums[j];
            }
            printf("%d\n", subtotal);
            exit(EXIT_SUCCESS);
        }
    }

    for (i = 0; i < n; i++) {
        int status;
        wait(&status);
    }

    return 0;
}
