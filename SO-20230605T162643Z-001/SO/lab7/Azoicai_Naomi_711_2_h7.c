#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024 // size of the buffer used for reading and writing data

void send_file_list(int pipe_write, const char *directory_name) {
    DIR *directory = opendir(directory_name); // open the directory
    if (directory == NULL) {
        char *error_message = "Verzeichnis nicht gefunden.";
        write(pipe_write, error_message, strlen(error_message) + 1); // send error through pipe
        return;
    }

    struct dirent *entry;
    char *file_list = malloc(BUFFER_SIZE); // allocate memory for file list buffer
    memset(file_list, 0, BUFFER_SIZE);      // clear the file list buffer

    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type == DT_REG) { // include regular files in file list
            char *file_extension = strrchr(entry->d_name, '.');
            if (file_extension != NULL && strcmp(file_extension, ".txt") == 0) {
                if (strlen(file_list) + strlen(entry->d_name) + 2 < BUFFER_SIZE) {
                    strncat(file_list, entry->d_name, strlen(entry->d_name)); // append file name to file list
                    strcat(file_list, "\n");                                  // add a newline character
                }
            }
        } else if (entry->d_type == DT_DIR) { // directory found
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                // create the full path of the subdirectory
                char subdirectory_path[BUFFER_SIZE];
                snprintf(subdirectory_path, BUFFER_SIZE, "%s/%s", directory_name, entry->d_name);

                // recursively send file list for the subdirectory
                send_file_list(pipe_write, subdirectory_path);
            }
        }
    }

    closedir(directory);                           // close directory
    write(pipe_write, file_list, strlen(file_list)); // send the file list through the pipe
    free(file_list);                                // free the allocated memory for the file list
}

int main() {
    int pipe1[2], pipe2[2]; // pipes used for communication between client and server processes
    pid_t child_pid;

    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        perror("Fehler beim Erstellen der Pipes");
        exit(EXIT_FAILURE);
    }

    child_pid = fork(); // create a child process
    if (child_pid < 0) {
        perror("Fehler beim Erstellen des Kindprozesses");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // child process (Client)
        close(pipe1[0]); // close read end of pipe1 since the child only writes to it
        close(pipe2[1]); // close write end of pipe 2 since the child only reads from it

        char directory_name[BUFFER_SIZE];
        memset(directory_name, 0, BUFFER_SIZE);
        read(pipe2[0], directory_name, BUFFER_SIZE); // read the directory name from the parent process

        send_file_list(pipe1[1], directory_name); // send file list or error message to parent process

        close(pipe1[1]); // close the write end of pipe1
        close(pipe2[0]); // close the read end of pipe2
        exit(EXIT_SUCCESS);
    } else {
        // parent process (Server)
        close(pipe1[1]); // close the write end of pipe1 since the parent only reads from it
        close(pipe2[0]); // close the read end of pipe2 since the parent only writes to it

        char directory_name[BUFFER_SIZE];
        printf("Geben Sie den Verzeichnisnamen ein: ");
        fgets(directory_name, BUFFER_SIZE, stdin);                    // read the directory name from the user input
        directory_name[strcspn(directory_name, "\n")] = '\0';         // remove newline character

        write(pipe2[1], directory_name, strlen(directory_name) + 1);  // send the directory name to the child process

        char response[BUFFER_SIZE];
        memset(response, 0, BUFFER_SIZE); // clear the response buffer
        read(pipe1[0], response, BUFFER_SIZE); // read the file list or error message from the child process

        if (strlen(response) > 0)
            printf("Dateiliste:\n%s\n", response); // print the file list
        else
            printf("Fehler: %s\n", response); // error

        close(pipe1[0]); // close read end of pipe1
        close(pipe2[1]); // close write end of pipe2
    }

    return 0;
}
