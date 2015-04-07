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

	if(argc == 1) {	// ��ɾ� �ڿ� ���ڰ� ���� �� 
		while((nread = read(0, buf, BUFSIZ)) > 0) {	// ǥ���Է����� �ް�
			if(write(1, buf, nread) < nread) {	// ǥ��������� ����
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
	
	if(argc > 1) {	// ��ɾ� �ڿ� ���ڰ� ���� ��
		for(i = 1; i < argc; i++) {	//
			if((infile = open(argv[i], O_RDONLY)) == -1) {	// ������ read-only�� ����
				printf("open error\n");
				exit(1);
			}

			while((nread = read(infile, buf, BUFSIZ)) > 0) {	// ���Ͽ��� �Է¹ް�
				if(write(1, buf, nread) < nread) {	// ǥ�����
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