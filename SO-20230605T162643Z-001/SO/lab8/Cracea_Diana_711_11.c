#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

#define FIFO_FILE "pipefile"
#define BUFFER_SIZE 1024

void calculateTotalBytes(const char* directory, long long* totalBytes, long long* sumBytes) {
    DIR* dir = opendir(directory);

    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char path[BUFFER_SIZE];
            snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

            struct stat fileStat;
            if (stat(path, &fileStat) == 0) {
                *totalBytes += fileStat.st_size;
                
                if (strstr(entry->d_name, ".txt") != NULL) {
                    *sumBytes += fileStat.st_size;
                }
            }
        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char path[BUFFER_SIZE];
            snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
            calculateTotalBytes(path, totalBytes, sumBytes);
        }
    }

    closedir(dir);
}

int main() {
    mkfifo(FIFO_FILE, 0666);

    int fd = open(FIFO_FILE, O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process (server)
        char dirname[BUFFER_SIZE];
        read(fd, dirname, sizeof(dirname));

        long long totalBytes = 0;
        long long sumBytes = 0;
        calculateTotalBytes(dirname, &totalBytes, &sumBytes);

        close(fd);
        unlink(FIFO_FILE);

        printf("Sum of bytes in text files in directory '%s': %lld\n", dirname, sumBytes);

    } else {
        // Parent process (client)
        char dirname[BUFFER_SIZE];
        printf("Enter directory name: ");
        fgets(dirname, sizeof(dirname), stdin);
        dirname[strcspn(dirname, "\n")] = '\0';

        write(fd, dirname, strlen(dirname) + 1);

        close(fd);
    }

    return 0;
}

