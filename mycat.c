#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

void main(int argc, char **argv) {
	int i, infile;
	ssize_t nread;
	char buf[BUFSIZ];

	if(argc == 1) {	// 명령어 뒤에 인자가 없을 때 
		while((nread = read(0, buf, BUFSIZ)) > 0) {	// 표준입력으로 받고
			if(write(1, buf, nread) < nread) {	// 표준출력으로 보냄
				printf("write error\n");
				exit(1);
			}
		}

		if(nread == -1) {
			printf("read error\n");
			exit(1);
		}

		exit(0);
	}
	
	if(argc > 1) {	// 명령어 뒤에 인자가 있을 때
		for(i = 1; i < argc; i++) {	//
			if((infile = open(argv[i], O_RDONLY)) == -1) {	// 파일을 read-only로 열기
				printf("open error\n");
				exit(1);
			}

			while((nread = read(infile, buf, BUFSIZ)) > 0) {	// 파일에서 입력받고
				if(write(1, buf, nread) < nread) {	// 표준출력
					close(infile);
					printf("write error\n");
					exit(1);
				}
			}

			close(infile);

			if(nread == -1) {
				printf("read error\n");
				exit(1);
			}
		}
		
		exit(0);
	}
}