#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int line, words;
	char ch;

	//Open file
	FILE *file = fopen(argv[1], "r");
	if (file == NULL)
	{
		perror("Error");
		return 1;
	}

	line = 1;
	words = 0;

	ch = fgetc(file);
	while (ch != EOF)
	{
		if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0')
			words++;
		if (ch == '\n' || ch == '\0')
		{
			printf("Line %d: %d words", line, words);
			words = 0;
			line++;
			printf("\n");
		}
		ch = fgetc(file);
	}

	fclose(file);

	return 0;
}
