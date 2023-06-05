#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 256

void error(const char *msg) {
    perror(msg);
    exit(1);
}

// Funcție pentru procesul server
void serverProcess(int pipefd[2]) {
    close(pipefd[1]); // Închide capătul de scriere al pipe-ului

    char directoryName[BUFFER_SIZE];
    read(pipefd[0], directoryName, BUFFER_SIZE); // Citeste numele directorului din pipe

    close(pipefd[0]); // Închide capătul de citire al pipe-ului

    if (access(directoryName, F_OK) == 0) { // Verifică existența directorului
        printf("Conținutul directorului %s:\n", directoryName);
        char command[BUFFER_SIZE + 7];
        snprintf(command, sizeof(command), "ls -l %s", directoryName); // Construiește comanda pentru a afișa conținutul directorului
        system(command); // Execută comanda în shell
    } else {
        printf("Directorul %s nu există.\n", directoryName);
    }
}

// Funcție pentru procesul client
void clientProcess(int pipefd[2]) {
    close(pipefd[0]); // Închide capătul de citire al pipe-ului

    char directoryName[BUFFER_SIZE];
    printf("Introduceți numele directorului: ");
    fgets(directoryName, BUFFER_SIZE, stdin); // Citește numele directorului de la utilizator

    directoryName[strcspn(directoryName, "\n")] = '\0'; // Elimină caracterul newline ('\n') din numele directorului

    write(pipefd[1], directoryName, strlen(directoryName) + 1); // Scrie numele directorului în pipe

    close(pipefd[1]); // Închide capătul de scriere al pipe-ului
}

int main() {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) { // Creează pipe-ul
        error("Eroare la crearea pipe-ului");
    }

    pid = fork(); // Creează un nou proces copil
    if (pid == -1) {
        error("Eroare la crearea procesului server");
    } else if (pid == 0) { // Procesul copil devine serverul
        serverProcess(pipefd);
        exit(0);
    }

    clientProcess(pipefd); // Procesul părinte devine clientul

    wait(NULL); // Așteaptă terminarea procesului server

    return 0;
}

