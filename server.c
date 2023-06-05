#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define FIFO_PATH "myfifo"
#define BUFFER_SIZE 1024

int main() {
    int fifo_fd;
    char directory_name[100];
    char buffer[BUFFER_SIZE];
    char temp_buffer[256];

    // Create the FIFO (named pipe) if it doesn't exist
    mkfifo(FIFO_PATH, 0666);

    // Open the FIFO for reading
    fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Read the directory name from the FIFO
    read(fifo_fd, directory_name, sizeof(directory_name));

    // Close the FIFO
    close(fifo_fd);

    // SO newline character from the directory name
    int len = (int) strlen(directory_name);
    if (len > 0 && directory_name[len - 1] == '\n')
        directory_name[len - 1] = '\0';

    printf("Directory name: %s\n", directory_name);

    // Open the directory and get its contents
    DIR *dir = opendir(directory_name);
    if (dir == NULL) {
        perror("Failed to open directory");
        exit(EXIT_FAILURE);
    }

    // Read the directory contents and send them back to the client
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strlen(entry->d_name) + 2 > sizeof(temp_buffer)) {
            fprintf(stderr, "File name too long: %s\n", entry->d_name);
            continue;
        }
        snprintf(temp_buffer, sizeof(temp_buffer), "%s\n", entry->d_name);
        strcat(buffer, temp_buffer);
    }

    // Open the FIFO for writing
    fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    // Write the directory contents to the FIFO
    write(fifo_fd, buffer, strlen(buffer));

    // Close the FIFO
    close(fifo_fd);

    // Close the directory
    closedir(dir);

    return 0;
}
