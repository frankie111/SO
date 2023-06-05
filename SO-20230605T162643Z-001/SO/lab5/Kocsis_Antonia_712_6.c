#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Error: No filename provided\n");
        return 1;
    }

    char filename[20];
    char word[20];
    int occurrences = 0;
    strcpy(filename, argv[1]);
    strcpy(word, argv[2]);
    printf("Filename:  %s\n", filename);
    printf("Word:  %s\n", word);
    FILE *fileIn = fopen(filename, "r+");
    FILE *fileOut = fopen("temp.txt", "w");
    char line[100];
    while (fgets(line, sizeof(line), fileIn) != NULL) {
        char *auxWord = strtok(line, " \\t\\n");
        char newline[100] = "";
        while (auxWord != NULL) {
            if (strcmp(auxWord, word) == 0) {
                occurrences++;
            } else {
                strcat(newline, auxWord);
                strcat(newline, " ");
            }


            auxWord = strtok(NULL, " \t\n");  // Move to the next token
        }
        strcat(newline, "\n");
        fputs(newline, fileOut);
    }

    fclose(fileIn);
    fclose(fileOut);
    FILE *file = fopen(filename, "w");
    FILE *file2 = fopen("temp.txt", "r");
    char lines[100];
    while (fgets(lines, sizeof(line), file2) != NULL)
        fputs(lines, file);
    fclose(file);
    fclose(file2);

    printf("Occurrences: %d\n", occurrences);


    return 0;
}