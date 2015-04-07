#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

void main(int argc, char** argv) {
	int fd;
	char c[1];
	int num;
	int count = 0, start = 0;

	if(argc != 4) {
		printf("usage : mytail [option] [number] [file] ..\n");
		exit(1);
	}

	fd = open(argv[3], O_RDONLY);

	if(strcmp(argv[1], "-n") == 0) {
		while(read(fd, c, 1) > 0)
			if(*c == '\n')
				++count;
		++count;

		start = count - atoi(argv[2]) - 1;
		count = 0;
		lseek(fd, SEEK_SET, 0);

		while(read(fd, c, 1) > 0) {
			if(count >= start)
				putchar(*c);
			if(*c == '\n')
				++count;
		}
	}
	else if(strcmp(argv[1], "-c") == 0) {
		while(read(fd, c, 1) > 0)
			++count;

		start = count - atoi(argv[2]) - 1;
		count = 0;
		lseek(fd, SEEK_SET, 0);

		while(read(fd, c, 1) > 0) {
			if(count >= start)
				putchar(*c);
			++count;
		}
	}
	close(fd);
}
