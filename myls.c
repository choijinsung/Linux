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
#include <time.h>

static short octarray[9] = {0400, 0200, 0100,
							0040, 0020, 0010,
							0004, 0002, 0001};

static char perms[10] = "rwxrwxrwx";

void main(int argc, char **argv) {
	static DIR *dp = NULL;
	struct dirent *d;
	struct stat fstat;
	char descrip[10];
	char home[200] = "";
	char homecpy[200] = "";
	int printcnt = 0;
	int count = 0;
	int i;

	if(getcwd(home, 200) == NULL) {
		printf("can't get current working directory.\n");
		exit(1);
	}

	strcpy(homecpy, home);

	if((dp = opendir(homecpy)) == NULL) {
		printf("can't open directory.\n");
		exit(1);
	}

	if(argc == 1) {
		while(d = readdir(dp)) {
			if(*(d -> d_name) == '.')
				continue;
			++count;
		}

		printf("total %d\n", count);
		rewinddir(dp);

		while(d = readdir(dp)) {
			if(*(d -> d_name) == '.')
				continue;
			
			printf("%-15s", d -> d_name);
			++printcnt;
			
			if(printcnt % 5 == 0)
				printf("\n");
		}
	}
	else if(argc == 2) {
		if(!strcmp(argv[1], "-a")) {
			while(d = readdir(dp)) 
				++count;

			printf("total %d\n", count);
			rewinddir(dp);
			
			while(d = readdir(dp)) {
				printf("%-20s", d -> d_name);
				++printcnt;

				if(printcnt % 5 == 0)
					printf("\n");
			}
		}
		else if(!strcmp(argv[1], "-l")) {
			while(d = readdir(dp)) {
				if(*(d -> d_name) == '.')
					continue;
				++count;
			}

			printf("total %d\n", count);
			rewinddir(dp);

			while(d = readdir(dp)) {
				if(*(d -> d_name) == '.')
					continue;

				strcat(homecpy, "/");

				if(stat(strcat(homecpy, d -> d_name), &fstat) == -1) {
					if(errno == ENOENT)
						fprintf(stderr, "%s is not found.\n", homecpy);
					else if(errno == EACCES)
						fprintf(stderr, "No permission for %s\n", homecpy);
					exit(1);
				}
				if(S_ISDIR(fstat.st_mode))
					printf("d");
				else
					printf("-");

				for(i = 0; i < 9; i++) {
					if(fstat.st_mode & octarray[i])
						descrip[i] = perms[i];
					else
						descrip[i] = '-';
				}
				descrip[9] = '\0';
				printf("%-10s", descrip);
				printf("%5d%5d%5d", fstat.st_nlink, fstat.st_uid, fstat.st_gid);
				printf("%7ld%17s%12s\n", fstat.st_size, ctime(&(fstat.st_mtime)), d -> d_name);

				strcpy(homecpy, "\0");
				strcpy(homecpy, home);
			}
		}
		else if(!strcmp(argv[1], "-al") || !strcmp(argv[1], "-la")) {
			while(d = readdir(dp)) 
				++count;

			printf("total %d\n", count);
			rewinddir(dp);

			while(d = readdir(dp)) {
				strcat(homecpy, "/");

				if(stat(strcat(homecpy, d -> d_name), &fstat) == -1) {
					if(errno == ENOENT)
						fprintf(stderr, "%s is not found.\n", homecpy);
					else if(errno == EACCES)
						fprintf(stderr, "No permission for %s\n", homecpy);
					exit(1);
				}
				if(S_ISDIR(fstat.st_mode))
					printf("d");
				else
					printf("-");

				for(i = 0; i < 9; i++) {
					if(fstat.st_mode & octarray[i])
						descrip[i] = perms[i];
					else
						descrip[i] = '-';
				}
				descrip[9] = '\0';
				printf("%-10s", descrip);
				printf("%5d%5d%5d", fstat.st_nlink, fstat.st_uid, fstat.st_gid);
				printf("%7ld%17s%12s\n", fstat.st_size, ctime(&(fstat.st_mtime)), d -> d_name);

				strcpy(homecpy, "\0");
				strcpy(homecpy, home);
			}
		}
	}
	else if(argc > 2) {
		if(!strcmp(argv[1], "-l")) {
			while(d = readdir(dp)) {
				for(i = 2; i < argc; i++) {
					if(!strcmp(argv[i], d -> d_name)) {
						strcat(homecpy, "/");

						if(stat(strcat(homecpy, d -> d_name), &fstat) == -1) {
							if(errno == ENOENT)
								fprintf(stderr, "%s is not found.\n", homecpy);
							else if(errno == EACCES)
								fprintf(stderr, "No permission for %s\n", homecpy);
							exit(1);
						}
						if(S_ISDIR(fstat.st_mode))
							printf("d");
						else
							printf("-");

						for(i = 0; i < 9; i++) {
							if(fstat.st_mode & octarray[i])
								descrip[i] = perms[i];
							else
								descrip[i] = '-';
						}
						descrip[9] = '\0';
						printf("%-10s", descrip);
						printf("%5d%5d%5d", fstat.st_nlink, fstat.st_uid, fstat.st_gid);
						printf("%7ld%17s%12s\n", fstat.st_size, ctime(&(fstat.st_mtime)), d -> d_name);

						strcpy(homecpy, "\0");
						strcpy(homecpy, home);
					}
				}
			}
		}
		else {
			while(d = readdir(dp)) {
				for(i = 1; i < argc; i++) {
					if(!strcmp(argv[i], d -> d_name)) {
						printf("%-15s", d -> d_name);
						++printcnt;

						if(printcnt % 5 == 0)
							printf("\n");
					}
				}
			}
		}
	}

	printf("\n");
	closedir(dp);
	dp = NULL;
}
