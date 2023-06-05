
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

bool isPerfectNumber(int num){
	int sum = 0;
	for(int i = 1; i<num;i++){
		if(num%i == 0){
			sum += i;
		}
	}
	if(sum == num) return true;
	return false;
}

int main(int argc, char* argv[]){
	int status;

	if(argc != 2) {
		printf("Falsche Anzahl Argumente");
	}
	pid_t pid, pid1;
	int n = atoi(argv[1]);
	if((pid = fork())<0){
		perror("Error");
		exit(1);
	}

	if(pid == 0){
		// Kind code
			if((pid1 = fork())<0){
				perror("Error");
				exit(1);
			}
			if(pid1 == 0){
				//Kind Kind code
				for(int i = n/3*2;i<n; i++){
					if(isPerfectNumber(i)) printf("%d ", i);
				}
				exit(0);
			}
		for(int i = n/3;i< n/3*2; i++){
			if(isPerfectNumber(i)) printf("%d ", i);
		}		

		wait(&status);	
		exit(0);
	}
	for(int i = 1;i< n/3; i++){
		if(isPerfectNumber(i)) printf("%d ", i);
	}
	wait(&status);
	exit(0);
}
