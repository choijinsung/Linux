#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

int strmatch(const char *s1, const char *s2) {
	int diff = strlen(s1) - strlen(s2);

	if(strlen(s1) > strlen(s2))
		return (strcmp(&s1[diff], s2) == 0);
	else
		return 0;
}

void sel_rm(char *suffix, char *dirname) {
	static DIR *dp = NULL;
	struct dirent *d;
	char dircpy[200] = "";
	char home[200] = "";
	char homecpy[200] = "";
	int i;
	
	if(getcwd(home, 200) == NULL)
		printf("can't get current working directory\n");

	strcpy(dircpy, dirname);
	strcat(home, "/");
	strcat(home, dircpy);
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

		if(strmatch(d -> d_name, suffix)) {
			strcat(homecpy, "/");
			remove(strcat(homecpy, d -> d_name));
			
			strcpy(homecpy, "\0");
			strcpy(homecpy, home);
		}
	}
	closedir(dp);
	dp = NULL;
}

void rm_dir(char *dirname) {
	static DIR *dp = NULL;
	struct dirent *d;
	struct stat fstat;
	char dircpy[200] = "";
	char home[200] = "";
	char homecpy[200] = "";
	int i;

	if(getcwd(home, 200) == NULL)
		printf("can't get current working directory\n");

	strcpy(dircpy, dirname);
	strcat(home, "/");
	strcat(home, dircpy);
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
		if(S_ISDIR(fstat.st_mode)) {
			strcat(dircpy, "/");
			rm_dir(strcat(dircpy, d -> d_name));
		}
		else 
			remove(homecpy);

		strcpy(homecpy, "\0");
		strcpy(homecpy, home);
	}
	remove(homecpy);
}

int cond_rm_dir(char *dirname) {
	static DIR *dp = NULL;
	struct dirent *d;
	struct stat fstat;
	char dircpy[200] = "";
	char home[200] = "";
	char homecpy[200] = "";
	int i;

	if(getcwd(home, 200) == NULL)
		printf("can't get current working directory\n");
	
	strcpy(dircpy, dirname);
	strcat(home, "/");
	strcat(home, dircpy);
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
			printf("cannot get directory entry information.\n");
			return 0;
		}
		if(S_ISDIR(fstat.st_mode)) {
			strcat(dircpy, "/");
			if(!cond_rm_dir(strcat(dircpy, d -> d_name))) 
				return 0;
		}
		else {
			printf("something exists in this directory.\n");
			return 0;
		}
		strcpy(homecpy, "\0");
		strcpy(homecpy, home);
	}
	remove(homecpy);
	return 1;
}

void main(int argc, char **argv) {
	int i;
	char dirname[200];
	struct stat fstat;

	if(argc < 2) {
		printf("command usage : myrm [OPTION] ... FILE ...\n");
		exit(1);
	}
	if(strcmp(argv[1], "-rf") == 0) {
		if(argc == 2) {
			printf("command usage : myrm [OPTION] ... FILE ...\n");
			exit(1);
		}
		if(*(argv[2]) == '*') {
			getcwd(dirname, 200);
			sel_rm(++argv[2], dirname);
		} else {
			for(i = 2; i < argc; i++) {
				if(stat(argv[i], &fstat) == -1) {
					if(errno == ENOENT)
						fprintf(stderr, "%s is not found.\n", argv[i]);
					else if(errno == EACCES)
						fprintf(stderr, "No permission for %s\n", argv[i]);
					exit(1);
				}
				if(S_ISDIR(fstat.st_mode)) 
					rm_dir(argv[i]);
				else 
					remove(argv[i]);
			}
		}
	}
	else if(strcmp(argv[1], "-f") == 0){
		if(argc == 2) {
			printf("command usage : myrm [OPTION] ... FILE ...\n");
			exit(1);
		} 
		if(*(argv[2]) == '*') {
			getcwd(dirname, 200);
			sel_rm(++argv[2], dirname);
		} else {
			for(i = 2; i < argc; i++) {
				if(stat(argv[i], &fstat) == -1) {
					if(errno == ENOENT)
						fprintf(stderr, "%s is not found.\n", argv[i]);
					else if(errno == EACCES)
						fprintf(stderr, "No permission for %s\n", argv[i]);
					exit(1);
				}
				if(S_ISDIR(fstat.st_mode)) { 
					cond_rm_dir(argv[i]);
				}
				else {
					remove(argv[i]);
				}
			}
		}
	}
}
