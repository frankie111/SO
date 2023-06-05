#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1024 

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "%s file1 file2\n", argv[0]);
        return 1;
    }

    char *filename1 = argv[1];
    char *filename2 = argv[2];

    FILE *file1 = fopen(filename1, "r");
    if (file1 == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename1);
        return 1;
    }

    FILE *file2 = fopen(filename2, "r");
    if (file2 == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename2);
        return 1;
    }

    int num_blocks1 = 0;
    int num_blocks2 = 0;
    int num_identical_blocks = 0;

    char *buffer1 = (char*) malloc(BLOCK_SIZE);
    char *buffer2 = (char*) malloc(BLOCK_SIZE);

    while (1) {
        size_t bytes_read1 = fread(buffer1, 1, BLOCK_SIZE, file1);
        size_t bytes_read2 = fread(buffer2, 1, BLOCK_SIZE, file2);

        if (bytes_read1 != bytes_read2 || memcmp(buffer1, buffer2, bytes_read1) != 0) {
        } else {
            num_identical_blocks++;
        }

        if (bytes_read1 < BLOCK_SIZE) {
            break;
        } else {
            num_blocks1++;
        }

        if (bytes_read2 < BLOCK_SIZE) {
            break;
        } else {
            num_blocks2++;
        }
    }

    printf("%s: %d\n%s: %d\n%d\n", filename1, num_blocks1, filename2, num_blocks2, num_identical_blocks);

    free(buffer1);
    free(buffer2);
    fclose(file1);
    fclose(file2);

    return 0;
}
