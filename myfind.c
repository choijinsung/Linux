#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

typedef struct {
	char type[200];
	int perm;
	int type_ck;
	int perm_ck;
}option;

void myfind(char *home, option op) {
	static DIR *dp = NULL;
	struct dirent *d;
	struct stat fstat;
	char homecpy[200] = "";
	int mask = 0777;
	int i;

	strcpy(homecpy, home);

	if((dp = opendir(homecpy)) == NULL) {
		printf("can't open directory\n");
		return;
	}
	
	while(d = readdir(dp)) {
		if(d -> d_ino == 0)
			continue;

		if(strcmp(d -> d_name, ".") == 0)
			continue;
		
		if(strcmp(d -> d_name, "..") == 0)
			continue;
	
		strcat(homecpy, "/");

		if(stat(strcat(homecpy, d -> d_name), &fstat) == -1) {
			if(errno == ENOENT)
				fprintf(stderr, "%s is not found.\n", homecpy);
			else if(errno == EACCES)
				fprintf(stderr, "No permission for %s\n", homecpy);
			exit(1);
		}

		printf("%s\n", homecpy);
		
		if(S_ISDIR(fstat.st_mode)) 
			myfind(homecpy, op);
		
		if(op.type_ck) {
			if(op.perm_ck) {
				if(S_ISDIR(fstat.st_mode) && !(strcmp(op.type, "f"))) {
					strcpy(homecpy, "\0");
					strcpy(homecpy, home);
					continue;
				}
				if(!S_ISDIR(fstat.st_mode) && !(strcmp(op.type, "d"))) {
					strcpy(homecpy, "\0");
					strcpy(homecpy, home);
					continue;
				}
				if(fstat.st_mode & mask != op.perm) {
					strcpy(homecpy, "\0");
					strcpy(homecpy, home);
					continue;
				}
				printf("%s\n", homecpy);
			} else {
				if(S_ISDIR(fstat.st_mode) && !(strcmp(op.type, "f"))) {
					strcpy(homecpy, "\0");
					strcpy(homecpy, home);
					continue;
				}
				if(!S_ISDIR(fstat.st_mode) && !(strcmp(op.type, "d"))) {
					strcpy(homecpy, "\0");
					strcpy(homecpy, home);
					continue;
				}
				printf("%s\n", homecpy);
			}
		} else {
			if(op.perm_ck) {
				if(fstat.st_mode & mask != op.perm) {
					strcpy(homecpy, "\0");
					strcpy(homecpy, home);
					continue;
				}
				printf("%s\n", homecpy);
			}
		}
		strcpy(homecpy, "\0");
		strcpy(homecpy, home);
	}
	closedir(dp);
}

void main(int argc, char **argv) {
	int i, j;
	option op;
	char permstr[200] = "";
	op.type_ck = 0;
	op.perm_ck = 0;

	char home[200] = "";
	if(getcwd(home, 200) == NULL) {
		printf("can't get current working directory\n");
		exit(1);
	}

	if(argc < 4) {
		printf("usage : find path [-option optionvalue] ..\n");
		exit(1);
	}

	if(*argv[1] == '/') {
		strcpy(home, "\0");
		strcpy(home, argv[1]);
	} 
	else if(*argv[1] == '.' && *(argv[1]+1) == '/') {
		strcat(home, argv[1]+1);
	} else {
		strcat(home, "/");
		strcat(home, argv[1]);
	}
	if(home[strlen(home)-1] == '/')
		home[strlen(home)-1] = '\0';
	
	for(i = 2; i < argc; i = i+2) {
		if(strcmp(argv[i], "-type") == 0) {
			strcpy(op.type, "");
			strcpy(op.type, argv[i+1]);
			op.type_ck = 1;
		} else if(strcmp(argv[i], "-perm") == 0) {
			strcpy(permstr, argv[i+1]);
			sscanf(permstr, "%o", &op.perm);
			op.perm_ck = 1;
		}
	}
	
	myfind(home, op);
}
