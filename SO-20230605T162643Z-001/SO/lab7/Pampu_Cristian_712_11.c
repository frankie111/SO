#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

// Function executed by the child process (the "server").
// It reads the directory name from the pipe, calculates the total size of files
// in that directory, and then writes this number back to the pipe.
void server_function(int pipefd_in[2], int pipefd_out[2]) {
    DIR *d;
    struct dirent *dir;
    struct stat buf;
    int total_bytes = 0;

    // Buffer for reading directory name from pipe
    char dir_name[256];
    // Read directory name from pipe
    read(pipefd_in[0], dir_name, sizeof(dir_name));

    // Open directory
    d = opendir(dir_name);
    if (d) {
        // Iterate over all entries in directory
        while ((dir = readdir(d)) != NULL) {
            // Get stats for entry
            stat(dir->d_name, &buf);
            // If entry is a regular file, add its size to total
            if (S_ISREG(buf.st_mode)) {
                total_bytes += buf.st_size;
            }
        }
        closedir(d);
    }

    // Close reading pipe end
    close(pipefd_in[0]);
    // Close writing pipe end for the input pipe
    close(pipefd_in[1]);

    // Write total bytes to pipe
    dprintf(pipefd_out[1], "%d", total_bytes);
}

// Function executed by the parent process (the "client").
// It creates a pipe, forks a new process, writes the directory name to the pipe,
// and then reads back the total file size from the pipe.
void client_function() {
    int pipefd_in[2], pipefd_out[2];
    pipe(pipefd_in);
    pipe(pipefd_out);

    // Directory to calculate size of
    char* dir_name = ".";

    // Write directory name to pipe
    write(pipefd_in[1], dir_name, strlen(dir_name) + 1);
    // Close writing end of the input pipe
    close(pipefd_in[1]);

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        server_function(pipefd_in, pipefd_out);
        exit(0);
    } else {
        // Parent process

        // Buffer for reading total bytes from pipe
        char buf[100];
        // Read total bytes from pipe
        read(pipefd_out[0], buf, sizeof(buf));

        printf("The total size of all files in directory %s is: %s bytes\n", dir_name, buf);

        wait(NULL); // Wait for child to finish
    }
}

int main() {
    client_function();
    return 0;
}
