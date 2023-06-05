#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *toDelete = argv[1];
    FILE *file = fopen(argv[2], "r"); 
    int counter = 0;
    char text[100];
    char end[100];
    int end_len = 0;

    while (fgets(text, 100, file)) {
        char *cuvant = strtok(text, " ");
        while (cuvant != NULL) {
            if (strcmp(cuvant, toDelete) == 0) {
                counter++;
            } else {
                strcpy(end + end_len, cuvant);
                end_len += strlen(cuvant);
                strcpy(end + end_len, " \0");
                end_len++;
            }
            cuvant = strtok(NULL, " ");
        }
    }
    fclose(file);
    file = fopen(argv[2], "w");
    fputs(end, file);
    fclose(file);
    printf("%d\n", counter);

    return 0;
}
