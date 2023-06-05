#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 100
#define FIFO_PATH "/tmp/lab8_fifo"

void inlocuireVocale(char *linie) {
    while (*linie != '\0') {
        switch (*linie) {
            case 'a':
            case 'A':
                *linie = '1';
                break;
            case 'e':
            case 'E':
                *linie = '2';
                break;
            case 'i':
            case 'I':
                *linie = '3';
                break;
            case 'o':
            case 'O':
                *linie = '4';
                break;
            case 'u':
            case 'U':
                *linie = '5';
                break;
        }
        linie++;
    }
}

void afisareLiniialfanumerice(char *linie) {
    while (*linie != '\0') {
        if (isalnum(*linie) || *linie == ' ') {
            putchar(*linie);
        }
        linie++;
    }
}

int main() {
    char linie[MAX_LINE_LENGTH];
    int fifo_fd;
    pid_t pid;

    // Creați FIFO
    mkfifo(FIFO_PATH, 0666);

    pid = fork();
    if (pid == -1) {
        perror("Eroare la crearea procesului");
        return 1;
    }

    if (pid == 0) {
        // Procesul copil - Citirea din FIFO și afișarea liniilor alfanumerice
        fifo_fd = open(FIFO_PATH, O_RDONLY);
        if (fifo_fd == -1) {
            perror("Eroare la deschiderea FIFO");
            return 1;
        }

        printf("Linii alfanumerice:\n");
        while (read(fifo_fd, linie, sizeof(linie)) > 0) {
            linie[strcspn(linie, "\n")] = '\0';  // Eliminați caracterul newline
            afisareLiniialfanumerice(linie);
            printf("\n");
        }

        close(fifo_fd);
    } else {
        // Procesul părinte - Citirea din fișier, înlocuirea vocalelor și scrierea în FIFO
        fifo_fd = open(FIFO_PATH, O_WRONLY);
        if (fifo_fd == -1) {
            perror("Eroare la deschiderea FIFO");
            return 1;
        }

        FILE *fisier;
        fisier = fopen("lab8.txt", "r");
        if (fisier == NULL) {
            printf("Eroare la deschiderea fișierului lab8.txt");
            return 1;
        }

        // Procesul 1 - Citirea din fișier și afișarea liniilor cu maxim 15 caractere
        printf("Linii cu maxim 15 caractere:\n");
        while (fgets(linie, sizeof(linie), fisier)) {
            linie[strcspn(linie, "\n")] = '\0';  // Eliminați caracterul newline
            if (strlen(linie) <= 15) {
                printf("%s\n", linie);
            }
        }

        // Închideți fișierul de intrare
        fclose(fisier);

        // Deschideți din nou fișierul pentru procesul 2
        fisier = fopen("lab8.txt", "r");
        if (fisier == NULL) {
            printf("Eroare la deschiderea fișierului lab8.txt");
            return 1;
        }

        // Procesul 2 - Citirea din fișier, înlocuirea vocalelor și scrierea în FIFO
        printf("\nLinii modificate:\n");
        while (fgets(linie, sizeof(linie), fisier)) {
            linie[strcspn(linie, "\n")] = '\0';  // Eliminați caracterul newline
            inlocuireVocale(linie);
            printf("%s\n", linie);
            write(fifo_fd, linie, sizeof(linie));  // Scrie în FIFO
        }

        close(fifo_fd);

        // Închideți fișierul
        fclose(fisier);

        wait(NULL);  // Așteaptă ca procesul copil să se termine

        // Ștergeți FIFO
        unlink(FIFO_PATH);
    }

    return 0;
}

