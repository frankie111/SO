#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLOCK_SIZE 10

// comparison function for qsort
int cmpfunc(const void *a, const void *b) {
  return (*(char *)a - *(char *)b);
}
int main() {
int fd12[2], fd13[2], fd14[2], fd25[2], fd35[2];
pid_t pid1, pid2, pid3, pid4, pid5;

// Erzeugen der Pipes
if (pipe(fd12) < 0 || pipe(fd13) < 0 || pipe(fd14) < 0 || pipe(fd25) < 0 || pipe(fd35) < 0) {
printf("Fehler beim Erzeugen der Pipes\n");
exit(1);
}

    char inputBuffer[1000];
    int bytesRead;
    int i = 2;

    printf("Enter text: ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    int bufferLength = strlen(inputBuffer);
    int numBlocks = bufferLength / BLOCK_SIZE;

    for (int j = 0; j < numBlocks; j++) {
        // Send block of 10 bytes to the appropriate pipe
        if (i == 2) {
            write(fd12[1], &inputBuffer[j * BLOCK_SIZE], BLOCK_SIZE);
        } else if (i == 3) {
            write(fd13[1], &inputBuffer[j * BLOCK_SIZE], BLOCK_SIZE);
        } else if (i == 4) {
            write(fd14[1], &inputBuffer[j * BLOCK_SIZE], BLOCK_SIZE);
        }

        i++;
        if (i > 4) {
            i = 2;
        }
    }
    close(fd12[1]);
    close(fd13[1]);
    close(fd14[1]);


// Erzeugen von Prozess 2
if ((pid2 = fork()) < 0) {
printf("Fehler beim Erzeugen von Prozess 2\n");
exit(1);
} else if (pid2 == 0) {
// Kindprozess: Prozess 2
    close(fd13[0]);
    close(fd14[0]);
    close(fd35[1]);
    close(fd12[1]);
    close(fd13[1]);
    close(fd14[1]);
    int sum = 0;
    char c;

    char buf[1024];
    ssize_t bytes_read;

    while ((bytes_read = read(fd12[0], buf, sizeof(buf))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            char c = buf[i];
            if (c >= '0' && c <= '9') {
                sum += (c - '0');
            }
            else if (c == '\0') {
                break; // end of string
            }
        }
    }

    write(fd25[1], &sum, sizeof(sum));


    close(fd12[0]);
    close(fd25[1]);


exit(0);
}

// Erzeugen von Prozess 3
if ((pid3 = fork()) < 0) {
printf("Fehler beim Erzeugen von Prozess 3\n");
exit(1);
} else if (pid3 == 0) {
// Kindprozess: Prozess 3
    close(fd14[0]);
    close(fd12[1]);
    close(fd13[1]);
    close(fd14[1]);
    close(fd12[0]);
    close(fd25[1]);

    char c;

    while (read(fd13[0], &c, 1) > 0) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            c = 'Z';
        }

        write(fd35[1], &c, 1);
    }

    close(fd13[0]);
    close(fd35[1]);

exit(0);
}

// Erzeugen von Prozess 4
if ((pid4 = fork()) < 0) {
printf("Fehler beim Erzeugen von Prozess 4\n");
exit(1);
} else if (pid4 == 0) {
// Kindprozess: Prozess 4

    close(fd12[1]);
    close(fd13[1]);
    close(fd14[1]);
    close(fd12[0]);
    close(fd25[1]);
    close(fd13[0]);
    close(fd35[1]);
    close(fd25[1]);
    close(fd35[1]);
  char buffer[10000];
  int bytes_read;

  bytes_read = read(fd14[0], buffer, 10000);

  // sort the input using qsort
  qsort(buffer, bytes_read, sizeof(char), cmpfunc);

  // print the sorted input to the console
  buffer[bytes_read]=0;
  printf("proc 4 %s \n", buffer);

  close(fd14[0]);
exit(0);
}

    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

if((pid5= fork()) < 0) {
printf("Fehler beim Erzeugen von Prozess 5\n");
exit(1);
} else if (pid5==0) {
// Kindprozess: Prozess 5
    close(fd14[0]);
    close(fd12[1]);
    close(fd13[1]);
    close(fd14[1]);
    close(fd12[0]);
    close(fd25[1]);
    close(fd13[0]);
    close(fd35[1]);

    int nr;
    char buf[10];
    printf("proc2: sum ");
    if (read(fd25[0], &nr, sizeof(nr)) > 0) {
        printf("%d\n", nr);
    }

    printf("proc3: ");
    while (read(fd35[0], buf, 1) > 0) {
        printf("%s", buf);
    }

    close(fd25[1]);
    close(fd35[1]);

exit(0);
}

    close(fd25[1]);
    close(fd35[1]);
//// Warten auf die Beendigung aller Kindprozesse
//waitpid(pid1, NULL, 0);
//waitpid(pid2, NULL, 0);
//waitpid(pid3, NULL, 0);
waitpid(pid4, NULL, 0);
waitpid(pid5, NULL, 0);

return 0;
}
