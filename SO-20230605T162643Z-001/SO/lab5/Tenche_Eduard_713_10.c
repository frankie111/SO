#include <stdio.h>


int main (int argc, char** argv)
{
    if(argc==1)
    {
        printf("Kein Datei gegeben");
        return -1;
    }
    FILE* file = fopen(argv[1], "r");
    int character;
    while(!feof(file))
    {
        character = fgetc(file);
        if(character=='\n')
        {
            printf("%c", character);
            character = fgetc(file);
        }
        if(character!='\n')
            printf("%c", character);
    }
    fclose(file);

    return 0;
}