#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int i, sum = 0;
    int pipefd[2]; // pipefd[0] - partea de citire, pipefd[1] - partea de scriere
    pid_t pid1, pid2;

    if (argc < 2) {
        printf("Introduceti cel putin un numar ca argument.\n");
        return 1;
    }

    if (pipe(pipefd) == -1) { //creare pipe
        perror("Eroare la crearea pipe-ului.\n");
        return 1;
    }

    pid1 = fork(); // primul copil
    if (pid1 == 0) {
        close(pipefd[0]); // copilul 1 inchide partea de citire a pipe-ului

        for (i = 1; i <= argc/2; i++) {
            sum += atoi(argv[i]) * atoi(argv[i]) * atoi(argv[i]); // aduna cubul primelor argc/2 argumente
        }

        write(pipefd[1], &sum, sizeof(sum)); // trimite suma partiala prin pipe

        close(pipefd[1]); // inchide partea de scriere a pipe-ului
        exit(0);
    } else if (pid1 < 0) {
        perror("Eroare la crearea primului copil.\n");
        return 1;
    }

    pid2 = fork(); // al doilea copil
    if (pid2 == 0) {
        close(pipefd[1]); // copilul 2 inchide partea de scriere a pipe-ului

        for (i = argc/2 + 1; i < argc; i++) {
            sum += atoi(argv[i]) * atoi(argv[i]) * atoi(argv[i]); // aduna cubul restului de argumente
        }

        int sub;
        read(pipefd[0], &sub, sizeof(sub)); // citeste suma partiala de la copilul 1
        sum += sub; // aduna suma partiala la suma totala

        printf("Suma cuburilor numerelor date este: %d\n", sum);

        close(pipefd[0]); // inchide partea de citire a pipe-ului
        exit(0);
    } else if (pid2 < 0) {
        perror("Eroare la crearea celui de-al doilea copil.\n");
        return 1;
    }

    close(pipefd[0]); // parintele inchide partea de citire a pipe-ului
    close(pipefd[1]); // parintele inchide partea de scriere a pipe-ului

    return 0;
}

