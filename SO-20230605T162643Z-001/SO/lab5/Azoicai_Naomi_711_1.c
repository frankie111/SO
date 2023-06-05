#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fp;
    char line[1000];//array of characters where we will store each line of text read from file

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error: Datei %s konnte nicht geoffnet werden\n", argv[1]);
        return 1;
    }

    while (fgets(line, sizeof(line), fp)) {
        printf("%s\n\n", line);
    }

    fclose(fp);

    return 0;
}
