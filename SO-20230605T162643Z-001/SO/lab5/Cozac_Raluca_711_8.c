#include <stdio.h>

int main(int argc, char *argv[]) {
    // Deschide fișierul dat ca primul argument în linia de comandă
    FILE *fp = fopen(argv[1], "r+");

    if (fp == NULL) {
        // În cazul în care fișierul nu a putut fi deschis
        printf("Nu se poate deschide fișierul.\n");
        return 1;
    }

    int num;
    // Parcurge fișierul
    while (fscanf(fp, "%d", &num) == 1) {
        // Verifică dacă numărul este impar
        if (num % 2 != 0) {
            // În cazul în care numărul este impar, mută cursorul fișierului la poziția curentă și scrie numărul în fișier
            fseek(fp, -sizeof(int), SEEK_CUR);
            fprintf(fp, "%d ", num);
        }
        else {
            // În cazul în care numărul este par, mută cursorul fișierului la următorul număr și sare peste acesta
            fseek(fp, sizeof(int), SEEK_CUR);
        }
    }

    // Închide fișierul
    fclose(fp);

    return 0;
}

