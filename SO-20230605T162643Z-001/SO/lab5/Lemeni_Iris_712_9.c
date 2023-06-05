#include <stdio.h>
#include <string.h>
#define MAX_LINE_LENGTH 100

void mergeAndSortFiles(FILE *file1, FILE *file2) {
    char line1[MAX_LINE_LENGTH];
    char line2[MAX_LINE_LENGTH];

    // Read the first line from each file
    fgets(line1, sizeof(line1), file1);
    fgets(line2, sizeof(line2), file2);

    // Merge and sort the lines until reaching the end of either file
    while (!feof(file1) && !feof(file2)) {
        if (strcmp(line1, line2) < 0) {
            printf("%s", line1);
            fgets(line1, sizeof(line1), file1);
        } else {
            printf("%s", line2);
            fgets(line2, sizeof(line2), file2);
        }
    }

    // Print any remaining lines from file1
    while (!feof(file1)) {
        printf("%s", line1);
        fgets(line1, sizeof(line1), file1);
    }

    // Print any remaining lines from file2
    while (!feof(file2)) {
        printf("%s", line2);
        fgets(line2, sizeof(line2), file2);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Please provide the names of two files as command line arguments.\n");
        return 1;
    }

    FILE *file1 = fopen(argv[1], "r");
    FILE *file2 = fopen(argv[2], "r");

    if (file1 == NULL || file2 == NULL) {
        printf("Error opening files.\n");
        return 1;
    }

    mergeAndSortFiles(file1, file2);

    fclose(file1);
    fclose(file2);

    return 0;
}
