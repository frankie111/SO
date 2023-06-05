#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

void replaceVowelsWithDigits(char* line, int len)
{
    for (int i=0; i < len; i++)
    {
        switch (line[i])
        {
            case 'a':
            case 'A':
                line[i] = '1';
                break;
            case 'e':
            case 'E':
                line[i] = '2';
                break;
            case 'i':
            case 'I':
                line[i] = '3';
                break;
            case 'o':
            case 'O':
                line[i] = '4';
                break;
            case 'u':
            case 'U':
                line[i] = '5';
                break;
        }
    }
}

bool checkIfLineIsAlphaNum(char* line, int len)
{
//    write(STDOUT_FILENO, "Din alfanum\n", strlen("Din alfanum\n"));
//    write(STDOUT_FILENO, line, strlen(line));
    int i = 0;
    while (line[i] != '\0')
    {
        if (isalnum(line[i]) == 0)
        {
            return false;
        }
        i++;
    }
    return true;
}

void printAlphaNum(char* linesConcatenated, int len)
{
    char* line = strtok(linesConcatenated, "\n");
    while (line != NULL)
    {
        if (checkIfLineIsAlphaNum(line, strlen(line)))
        {
            write(STDOUT_FILENO, line, strlen(line));
            write(STDOUT_FILENO, "\n", 1);
        }
        line = strtok(NULL, "\n");
    }
}

int main(int argc, char** argv)
{
    // Open pipe 12
    int pipe12[2];
    if (pipe(pipe12) == -1)
    {
        perror("Pipe12 failed to open\n");
        return 1;
    }
    // Open pipe 13
    int pipe13[2];
    if (pipe(pipe13) == -1)
    {
        perror("Pipe13 failed to open\n");
        return 1;
    }


    // Used for test
    FILE* file;
    file = fopen("f1Preview.txt", "w");

    // Process 1
    write(STDOUT_FILENO, "\nStart process 1:\n", strlen("\nStart process 1:\n"));
    char line[BUFFER_SIZE];
    while(1)
    {
        printf("Enter a line: ");
        fgets(line, BUFFER_SIZE, stdin);
        fflush(stdout);
        if (line[0] == '\n')
        {
            line[0] == '\0';
            fwrite(&line, 1, 1, file); //  test
            write(pipe12[1], line, 1);
            write(pipe13[1], line, 1);
            printf("Read ended in process 1...\n");
            break;
        }
        else
        {
            line[strlen(line) - 1] = '\n';
            if (strlen(line) <= 15)
            {
                fwrite(&line, strlen(line), 1, file); //  test
                write(pipe12[1], line, strlen(line));
                write(pipe13[1], line, strlen(line));
            }
        }
    }

    fclose(file);


    // Open process 2
    pid_t pid2 = fork();
    if (pid2 < 0)
    {
        perror("Fork failed error: ");
        exit(EXIT_FAILURE);
    }
    if (pid2 != 0) // P1
    {
        int status2;
        waitpid(pid2, &status2, EXIT_SUCCESS);

        // Create process 3
        pid_t pid3 = fork();
        if (pid3 < 0)
        {
            perror("Fork failed error: ");
            exit(EXIT_FAILURE);
        }
        if (pid3 != 0) // P1
        {
            close(pipe12[0]); // Close 12 read
            close(pipe13[0]); // Close 13 read
            close(pipe12[1]); // Close 12 write
            close(pipe13[1]); // Close 13 write

            int status3;
            waitpid(pid3, &status3, EXIT_SUCCESS);
        }
        else // P3
        {
            write(STDOUT_FILENO, "\nStart process 3:\n\n", strlen("\nStart process 3:\n\n"));
            close(pipe12[0]); // Close 12 read
            close(pipe12[1]); // Close 12 write
            close(pipe13[1]); // Close 13 write

            int bytes_read = read(pipe13[0], line, BUFFER_SIZE);
            write(STDOUT_FILENO, line, bytes_read);

            close(pipe13[0]); // Close 13 read
            write(STDOUT_FILENO, "End process 3:\n\n", strlen("\nEnd process 3:\n\n"));
            exit(EXIT_SUCCESS);
        }
    }
    else // P2
    {
        close(pipe12[1]); // Close 12 write
        close(pipe13[0]); // Close 13 read
        close(pipe13[1]); // Close 13 write

        // Open pipe 24
        int pipe24[2];
        if (pipe(pipe24) == -1)
        {
            perror("Pipe24 failed to open\n");
            return 1;
        }

        // Create process 4
        pid_t pid4 = fork();
        if (pid4 < 0)
        {
            perror("Fork failed error: ");
            exit(EXIT_FAILURE);
        }
        if (pid4 != 0) // P2
        {
            close(pipe24[0]); // Close 24 read

            int bytes_read = read(pipe12[0], line, BUFFER_SIZE);
            replaceVowelsWithDigits(line, strlen(line));
            write(pipe24[1], line, bytes_read);

            close(pipe12[0]); // Close 12 read
            close(pipe24[1]); // Close 24 write

            int status4;
            waitpid(pid4, &status4, EXIT_SUCCESS);
            exit(EXIT_SUCCESS);
        }
        else // P4
        {
            write(STDOUT_FILENO, "\nStart process 4:\n\n", strlen("\nStart process 4:\n\n"));
            close(pipe12[0]); // Close 12 read
            close(pipe24[1]); // Close 24 write

            int bytes_read = read(pipe24[0], line, BUFFER_SIZE);
            printAlphaNum(line, bytes_read);

            close(pipe12[0]); // Close 12 read
            close(pipe24[0]); // Close 24 read
            write(STDOUT_FILENO, "\nEnd process 4:\n\n", strlen("\nEnd process 4:\n\n"));
            exit(EXIT_SUCCESS);
        }
    }
	return 0;
}