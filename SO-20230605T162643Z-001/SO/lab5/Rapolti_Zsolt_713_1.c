#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        printf("%s\n", line);
    }
    fclose(file);
    return 0;
}

