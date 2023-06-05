#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
	int n, gcd, next;
	printf("Enter n: ");
	scanf("%d", &n);
	printf("\nNumber of numbers to read: %d \n", n);
	if (n == 0)
	{
		printf("%d", 0);
		return 0;
	}
	// Read first number
	int index = 1;
 	printf("Read number %d of %d: ", index, n);
	scanf("%d", &gcd);
	index++;
	while (index <= n)
	{
		int id = fork();
		if (id == -1)
		{
			perror("Fork failed error: ");
			exit(EXIT_FAILURE);
		}
		if (id == 0)
		{
			printf("Read number %d of %d: ", index, n);
			scanf("%d", &next);
			while(gcd != next)
			{
				if(gcd > next)
					gcd -= next;
				else
					next -= gcd;
			}
			index++;
			printf("Current gcd = %d \n", gcd);
		}
		else
		{
			wait(NULL);
			n = -1;
		}
	}
	return 0;
}