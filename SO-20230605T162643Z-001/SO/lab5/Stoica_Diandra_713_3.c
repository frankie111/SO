#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	int i;
	int count=0;
	char c[2];
	int fd = open(argv[1], O_RDONLY);
	if(fd==-1){
		perror("open");
		return 1;
	}
	while(read(fd, c, 1)){
		if(c[0]==' '){
			count=count+1;
		}
		if(c[0]=='\n'){
			count=count+1;
			printf("%d\n", count);
			count=0;
		}
	}
	if (count > 0){
		printf("%d\n", count);
	}
	close(fd);
	return 0;
}

