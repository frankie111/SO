#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <sys/errno.h>


#define MAX_LINES 10
#define BUFFER_SIZE 100

extern int errno;

#define PIPE_1 "fifo1"
#define PIPE_2 "fifo2"
#define PIPE_3 "fifo3"

int main() {

    if (mkfifo(PIPE_1, 0600)) {
        if (errno != EEXIST) {
            perror("Not created: PIPE_1");
            exit(1);
        }
    }


    if (mkfifo(PIPE_2, 0600)) {
        if (errno != EEXIST) {
            perror("Not created: PIPE_2");
            exit(1);
        }
    }


    if (mkfifo(PIPE_3, 0600)) {
        if (errno != EEXIST) {
            perror("Not created: PIPE_3");
            exit(1);
        }
    }

    pid_t pid2, pid3, pid4;

    int fd1 = open(PIPE_1, O_RDWR);
    int fd2 = open(PIPE_2, O_RDWR);
    int fd3 = open(PIPE_3, O_RDWR);

    // Read from user
    char buffer[100];


    char *lines[MAX_LINES];
    int line_count = 0;

    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        if (strcmp(buffer, "!exit\n") == 0) {
            printf("\n");
            break;
        }

        lines[line_count] = strdup(buffer);
        line_count++;

        if (line_count >= MAX_LINES) {
            printf("Too many lines\n");
            break;
        }
    }

    for (int i = 0; i < line_count; i++) {
        int is_alphanumeric = 1;
        int has_letter = 0;
//        printf(" len %d \n",strlen(lines[i]));
        for (int j = 0; j<strlen(lines[i]); j++) {
            if (lines[i][j] == '\n') {
                continue;  // Skip the newline character
            }
//            printf(" char:%c ",lines[i][j]);
            if (!isalnum(lines[i][j])) {
                is_alphanumeric = 0;
//                printf("n %c ",lines[i][j]);
            }

            if (isalpha(lines[i][j])) {
                has_letter = 1;
            }
        }

        if (is_alphanumeric) {
//            printf("alpha %s", lines[i]);
            write(fd1, lines[i], strlen(lines[i]) );
        }

        if (!has_letter) {
//            printf("not letter %s", lines[i]);
            write(fd2, lines[i], strlen(lines[i]) );
        }
    }

    write(fd1, " ", 1 );
    write(fd2, " ", 1 );

    if((pid2 = fork()) < 0) {
        printf("Not created: process 2\n");
        exit(1);
    } else if (pid2 == 0) {

        char buffer2[100];

        if(read(fd1, buffer2, 100) < 0)
        {
            perror("Error reading from PIPE1");
            exit(1);
        }

            for (int i = 0; i < strlen(buffer2); i++) {
                if (isdigit(buffer2[i])) {
                    buffer2[i] = '_';
                }
            }

            close(fd1);



            if (write(fd3, buffer2, strlen(buffer2)) < 0) {
                perror("Error writing to PIPE2");
                exit(1);
            }


        close(fd1);
        close(fd2);
        close(fd3);

        exit(0);
    }

    if ((pid3 = fork()) < 0) {
        printf("Not created: process 3\n");
        exit(1);
    } else if (pid3 == 0) {

        char buffer3[100];
        
        if (read(fd2, buffer3, 100) < 0) {
            perror("Error reading from PIPE2");
            exit(1);
        }

        printf("Process 3: %s", buffer3);
        printf("\n");

        close(fd1);
        close(fd2);
        close(fd3);

        exit(0);
    }

    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    if ((pid4 = fork()) < 0) {
        printf("Not created: process 4\n");
        exit(1);
    } else if (pid4 == 0) {


        char buffer4[100];

        if (read(fd3, buffer4, 100) < 0) {
            perror("Error reading from PIPE3");
            exit(1);
        }
        
        printf("Process 4: %s\n", buffer4);

        close(fd1);
        close(fd2);
        close(fd3);

        exit(0);
    }

    close(fd1);
    close(fd2);
    close(fd3);

    waitpid(pid4, NULL, 0);




    return 0;
}
