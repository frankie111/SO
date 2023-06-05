#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Please add two files\n");
        return 1;
    }

    char *file1_name = argv[1];
    char *file2_name = argv[2];

    FILE *file1 = fopen(file1_name, "r");
    if (file1 == NULL) {
        perror("failed to open file1");
        return 1;
    }

    FILE *file2 = fopen(file2_name, "r");
    if (file2 == NULL) {
        perror("failed to open file2");
        fclose(file1);
        return 1;
    }

    char line1[MAX_LINE_LENGTH];
    char line2[MAX_LINE_LENGTH];

    if (fgets(line1, MAX_LINE_LENGTH, file1) == NULL) {
        while (fgets(line2, MAX_LINE_LENGTH, file2) != NULL) {
            printf("%s", line2);
        }
        fclose(file1);
        fclose(file2);
        return 0;
    }

    if (fgets(line2, MAX_LINE_LENGTH, file2) == NULL) {
        printf("%s", line1);
        while (fgets(line1, MAX_LINE_LENGTH, file1) != NULL) {
            printf("%s", line1);
        }
        fclose(file1);
        fclose(file2);
        return 0;
    }

    while (1) {
        int cmp = strcmp(line1, line2);

        if (cmp <= 0) {
            printf("%s", line1);
            if (fgets(line1, MAX_LINE_LENGTH, file1) == NULL) {
                printf("%s", line2);
                while (fgets(line2, MAX_LINE_LENGTH, file2) != NULL) {
                    printf("%s", line2);
                }
                break;
            }
        } else {
            printf("%s", line2);
            if (fgets(line2, MAX_LINE_LENGTH, file2) == NULL) {
                printf("%s", line1);
                while (fgets(line1, MAX_LINE_LENGTH, file1) != NULL) {
                    printf("%s", line1);
                }
                break;
            }
        }
    }

    fclose(file1);
    fclose(file2);

    return 0;
}

