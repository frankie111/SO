#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int is_prime(int n)
{
    if(n==1)
        return 0;
    for(int i = 2; i < n; i++)
        if(n%i==0)
            return 0;
    return 1;
}

int main(){
    FILE* file = fopen("output.txt", "w");
    int input;
    scanf("%d", &input);
    for(int i = 2; i <= input; i++)
    {
        pid_t pid = fork();
        if(pid<0)
            exit(1);
        else if (pid==0)
        {
            if(is_prime(i))
                exit(0);
            else
                exit(1);
        }
    }
    for(int i = 2; i <= input; i++)
    {
        int status;
        wait(&status);

        if(WEXITSTATUS(status)==0)
            fprintf(file, "%d ", i);
    }

    fclose(file);

    return 0;
}