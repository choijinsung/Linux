#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <utime.h>

void main(int argc, char** argv) {
	int fd;
	struct utimbuf *timebuf = NULL;

	if(argc < 2) {
		printf("usage : mytouch [option] [filename]\n");
		exit(1);
	}

	fd = open(argv[1], O_CREAT);

	if(utime(argv[1], timebuf) < 0) {
		printf("utime error\n");
		exit(1);
	}
	
	close(fd);
}
