#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int n = argc - 1;
    pid_t pid1, pid2;
	int cubed_array[n];
    int sum = 0;
	if(argc < 2){
		printf("Not enough arguments \n");
	return 1;}
    // create first child process to calculate cubes and put them in the array
    pid1 = fork();
    if (pid1 < 0) {
        printf("Failed to create first child process\n");
        return 1;
    } if (pid1 == 0) {
        // this is the first child process
        for (int i = 1; i <= n; i++) {
            int cube = atoi(argv[i]) * atoi(argv[i]) * atoi(argv[i]);
			cubed_array[i-1] = cube;
        }
		pid2 = fork();
		if (pid2 < 0) {
        printf("Failed to create second child process\n");
        return 1;
    }
		if(pid2 == 0) {
		// this is the second child process, that sums up the elements inside the cubed array and prints the sum
			for (int i = 0; i < n; i++) {
            sum += cubed_array[i];
        }
		printf("Sum is %d \n",sum);
		exit(0);
		}
        exit(0);
    }


    // print that it ended,wait for children to finish
    wait(NULL);
    wait(NULL);
    printf("Done\n");
    return 0;
}