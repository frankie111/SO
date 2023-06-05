#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int gcd(int a, int b);

int main() {
    int n, i;
    printf("Wie viele Zahlen? ");
    scanf("%d", &n);
    int arr[n]; // Array zum Speichern der Zahlen
    printf("Schreibe bitte die %d Zahlen: ", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]); // Zahlen in das Array einlesen und speichern
    }

    pid_t pid1, pid2;
    int gcd1, gcd2, gcd_final;

    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        // Kind 1: ggT der ersten Hälfte des Arrays berechnen
        gcd1 = arr[0];
        for (i = 1; i < n / 2; i++) {
            gcd1 = gcd(gcd1, arr[i]);
        }
        exit(gcd1); // Kind 1 beendet und gibt den ggT als Exit-Status zurück
    }

    pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        // Kind 2: ggT der zweiten Hälfte des Arrays berechnen
        gcd2 = arr[n / 2];
        for (i = n / 2 + 1; i < n; i++) {
            gcd2 = gcd(gcd2, arr[i]);
        }
        exit(gcd2); // Kind 2 beendet und gibt den ggT als Exit-Status zurück
    }

    // Warten auf die Beendigung beider Kinder
    waitpid(pid1, &gcd1, 0);
    waitpid(pid2, &gcd2, 0);

    // Den endgültigen ggT berechnen
    gcd_final = gcd(gcd1 >> 8, gcd2 >> 8);

    printf("Der ggT der %d Zahlen ist %d\n", n, gcd_final);
    return 0;
}

int gcd(int a, int b) {
    if (b == 0) {
        return a;
    } else {
        return gcd(b, a % b);
    }
}
