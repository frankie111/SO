#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	char buffer1[1024],buffer2[1024];
	int blocks1=0,blocks2=0;
	int identical=0;
	if(argc<3)
	{
		printf("Not enough arguments");
		return 1;
	}
	
	FILE *fisier1=fopen(argv[1],"r");

        if(fisier1==NULL)
        {
                printf("Files didn't open");
                return 1;
        }

	FILE *fisier2=fopen(argv[2],"r");
	if(fisier2==NULL)
	{
		fclose(fisier1);
		printf("Files didn't open");
		return 1;
	}

	
size_t bytes_read1, bytes_read2;
while ((bytes_read1 = fread(buffer1, 1024, 1, fisier1)) > 0 &&
       (bytes_read2 = fread(buffer2, 1024, 1, fisier2)) > 0)
{
    blocks1++;
    blocks2++;

    if (memcmp(buffer1, buffer2, bytes_read1) == 0)
    {
        identical++;
    }
}
if(bytes_read1>0){
blocks1++;
}


if(bytes_read2>0){
	blocks2++;
}
while ((bytes_read1 = fread(buffer1, 1024, 1, fisier1)) > 0)
{
    blocks1++;
}


while ((bytes_read2 = fread(buffer2, 1024, 1, fisier2)) > 0)
{
    blocks2++;
}

	printf("%s: %d\n", argv[1], blocks1);
    	printf("%s: %d\n", argv[2], blocks2);
    	printf("Identical Blocks: %d\n", identical);
	fclose(fisier1);
	fclose(fisier2);
	
	return 0;
}
