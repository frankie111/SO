#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int isPerfect(int n)
{
    int sum = 0;
    for (int i = 1; i < n; i++)
    {
        if (n % i == 0)
        {
            sum += i;
        }
    }
    return (sum == n);
}

int main()
{
    int n;
    printf("Geben Sie eine Zahl (N) ein: ");
    scanf("%d", &n);

    printf("Suche nach perfekten Zahlen kleiner als %d\n", n);

    int num_processes = 3;
    int block_size = n / num_processes;

    for (int i = 0; i < num_processes; i++)
    {
        int pid = fork();
        if (pid == -1)
        {
            perror("Fehler bei fork()");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) // im Kindprozess
        {
            int start = i * block_size + 1;
            int end = (i + 1) * block_size;
            if (i == num_processes - 1)
            {
                end = n - 1;
            }
            for (int j = start; j <= end; j++)
            {
                if (isPerfect(j))
                {
                    printf("%d ist eine perfekte Zahl\n", j);
                }
            }
            exit(EXIT_SUCCESS);
        }
    }

    int status;
    for (int i = 0; i < num_processes; i++)
    {
        wait(&status);
    }

    return 0;
}
