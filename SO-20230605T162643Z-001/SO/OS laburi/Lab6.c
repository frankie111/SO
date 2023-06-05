#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int cargs,char ** argv){
if(cargs != 2){
printf("Wrong amount of arguments\n");
return 0;
}
int fdescriptor = open(argv[1],O_RDWR);
FILE* tempFile = tmpfile();
if((tempFile == NULL)||(fdescriptor == -1)){
printf("File-related work failed\n");
return 0;
}
char* buf[1];
int count = 0;

while(read(fdescriptor,buf,1)>0){
if(count % 2 != 0){
fwrite(buf,1,1,tempFile);
}
count = count + 1;
}

rewind(tempFile);
char allText[count];
int read_size,written_size,tempDescriptor;
tempDescriptor = fileno(tempFile);
read_size = read(tempDescriptor,allText,count);
if(read_size == -1){
printf("Error when reading from temp file\n");
return 0;
}
close(fdescriptor);
fdescriptor = open(argv[1], O_RDWR | O_TRUNC);
written_size = write(fdescriptor,allText,read_size);
if(written_size == -1){
printf ("Error when re-writing original file\n");
return 0;
}
fclose(tempFile);
close(tempDescriptor);
close(fdescriptor);
printf("Program finished succesfully\n");
return 0;

}
