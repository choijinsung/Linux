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
	struct dirent *d;
	static DIR *dp;
	int fd1, fd2;
	char path[200];
	int i;
	ssize_t nread;
	char buf[BUFSIZ];
	
	if((dp = opendir(argv[2])) == NULL) {
		if((fd1 = open(argv[1], O_RDONLY)) == -1) {
			printf("src file open error\n");
			exit(1);
		}
		if((fd2 = open(argv[2], O_WRONLY|O_CREAT, 0666)) == -1) {
			printf("dst file open error\n");
			exit(1);
		}
		
		while((nread = read(fd1, buf, BUFSIZ)) > 0) {
			if(write(fd2, buf, nread) < nread) {
				close(fd1);
				close(fd2);
				printf("write error\n");
				exit(1);
			}	
		}

		close(fd1);
		close(fd2);
		
		if(nread == -1) {
			printf("read error\n");
			exit(1);
		}
		
		remove(argv[1]);
		closedir(dp);
		exit(0);
	}
	
	strcpy(path, argv[2]);
	strcat(path, "/");
	strcat(path, argv[1]);
	
	if((fd1 = open(argv[1], O_RDONLY)) == -1) {
		printf("src file open error\n");
		exit(1);		
	}
	if((fd2 = open(path, O_WRONLY|O_CREAT, 0666)) == -1) {
		printf("dst file open error\n");
		exit(1);
	}
		
	while((nread = read(fd1, buf, BUFSIZ)) > 0) {
		if(write(fd2, buf, nread) < nread) {
			close(fd1);
			close(fd2);
			printf("write error\n");
			exit(1);	
		}
	}

	close(fd1);
	close(fd2);
		
	if(nread == -1) {
		printf("read error\n");
		exit(1);
	}
		
	remove(argv[1]);
	closedir(dp);
	exit(0);
}
