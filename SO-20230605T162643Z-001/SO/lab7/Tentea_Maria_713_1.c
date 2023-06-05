#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#define MAX_LENGTH 30
int main(){
int pipe12[2];
int pipe13[2];
char buffer[31];

int pid1, pid2, pid3;
if(pipe(pipe12) == -1 || pipe(pipe13) == -1){
	perror("error");
	exit(1);
}

if((pid1 = fork()) < 0){
	printf("fork error");
	exit(1);
}

else if(pid1 == 0){
	close(pipe12[0]);
	close(pipe13[0]);

	//prima cerinta
	//zeilein mit max 30 zeichen lesen
	char line[MAX_LENGTH + 1];
	 while(1){
                printf("Enter a line(max 30 characters)");
                fgets(line, sizeof(line), stdin);
                if(line[0] == '0'){
                        break;
                }
                if(strlen(line) < MAX_LENGTH){
                        //printf("Line too long!\n");
                        //continue;

		for(int i =0; i<strlen(line); i++){
			if(isalpha(line[i])){
				if (islower(line[i])){
                        line[i] = toupper(line[i]);}
				write(pipe13[1], &line[i] , 1);
				//printf("%d", i);
			}
			else if (isdigit(line[i])){
				write(pipe12[1], &line[i], 1);
			}

		}}
	}
	close(pipe12[1]);
	close(pipe13[1]);
	exit(0);
}
else{
	close(pipe12[1]);
	close(pipe13[1]);
    // Declare a buffer to store the received characters
    char receivedLines[MAX_LENGTH + 1];

    // Read and print the contents of pipe12
    printf("Contents of pipe12:\n");
    ssize_t bytesRead;
    while ((bytesRead = read(pipe12[0], receivedLines, sizeof(receivedLines) - 1)) > 0) {
        receivedLines[bytesRead] = '\0'; //pentru ca nu stie cand se termina si imi adauga prostii
        printf("%s", receivedLines);
    }
    printf("\n");

printf("Contents of pipe13:\n");
    while ((bytesRead = read(pipe13[0], receivedLines, sizeof(receivedLines) - 1)) > 0) {
        receivedLines[bytesRead] = '\0';
        printf("%s", receivedLines);
    }
    printf("\n");
	wait(NULL);
	exit(0);
}
}
