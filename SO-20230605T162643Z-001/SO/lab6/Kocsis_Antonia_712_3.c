#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
// Funktion zum Bestimmen des Minimalwerts in einem Teil der Liste
int findMin(int *arr, int start, int end) {
    int min = arr[start];
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

// Funktion zum Bestimmen des Maximalwerts in einem Teil der Liste
int findMax(int *arr, int start, int end) {
    int max = arr[start];
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Hauptprogramm
int main() {
    int N; // Anzahl der Zahlen
    int k; // Index des Elements
    printf("Geben Sie die Anzahl der Zahlen ein: ");
    scanf("%d", &N);

    int *arr = malloc(N * sizeof(int));
    printf("Geben Sie die Zahlen durch Leerzeichen getrennt ein: ");
    for (int i = 0; i < N; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Geben Sie den Index (k) des Elements ein, das Sie finden möchten: ");
    scanf("%d", &k);

    if (k >= N) {
        printf("Ungültiger Index!\n");
        free(arr);
        return 0;
    }

    int min, max;

    // Prozess zum Bestimmen des Minimalwerts
    int pid_min = fork();
    if (pid_min == 0) {
        min = findMin(arr, 0, N - 1);
        free(arr);
        exit(min);
    }

    // Prozess zum Bestimmen des Maximalwerts
    int pid_max = fork();
    if (pid_max == 0) {
        max = findMax(arr, 0, N - 1);
        free(arr);
        exit(max);
    }

    // Warten auf die Beendigung der Prozesse
    int status_min, status_max;
    waitpid(pid_min, &status_min, 0);
    waitpid(pid_max, &status_max, 0);

    if (WIFEXITED(status_min) && WIFEXITED(status_max)) {
        min = WEXITSTATUS(status_min);
        max = WEXITSTATUS(status_max);

        if (k == 0) {
            printf("Das k-te Element ist: %d\n", min);
        } else if (k == N - 1) {
            printf("Das k-te Element ist: %d\n", max);
        } else {
            int count = 0;
            int kthElement = min;

            while (count < k) {
                kthElement = max;
                for (int i = 0; i < N; i++) {
                    if (arr[i] > min && arr[i] < kthElement) {
                        kthElement = arr[i];
                    }
                }
                min = kthElement;
                count++;
            }

            printf("The k-th element is: %d\n", kthElement);

        }

        free(arr);
        return 0;
    }
}

