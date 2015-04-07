#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

int u = 0, g = 0, o = 0, a = 0;
int p = 0, m = 0, e = 0;
int r = 0, w = 0, x = 0;
int static_perm = 0;

void play_umode(char *path) {
	if(p == 1) {
		if(r)
			static_perm |= 0400;
		if(w)
			static_perm |= 0200;
		if(x)
			static_perm |= 0100;
	} else if(m == 1) {
		if(r)
			static_perm &= ~0400;
		if(w)
			static_perm &= ~0200;
		if(x)
			static_perm &= ~0100;
	} else if(e == 1) {
		if(!r)
			static_perm &= ~0400;
		if(!w)
			static_perm &= ~0200;
		if(!x)
			static_perm &= ~0100;
	}
}

void play_gmode(char *path) {
	if(p == 1) {
		if(r)
			static_perm |= 0040;
		if(w)
			static_perm |= 0020;
		if(x)
			static_perm |= 0010;
	} else if(m == 1) {
		if(r)
			static_perm &= ~0040;
		if(w)
			static_perm &= ~0020;
		if(x)
			static_perm &= ~0010;
	} else if(e == 1) {
		if(!r)
			static_perm &= ~0040;
		if(!w)
			static_perm &= ~0020;
		if(!x)
			static_perm &= ~0010;
	}
}

void play_omode(char *path) {
	if(p == 1) {
		if(r)
			static_perm |= 0004;
		if(w)
			static_perm |= 0002;
		if(x)
			static_perm |= 0001;
	} else if(m == 1) {
		if(r)
			static_perm &= ~0004;
		if(w)
			static_perm &= ~0002;
		if(x)
			static_perm &= ~0001;
	} else if(e == 1) {
		if(!r)
			static_perm &= ~0004;
		if(!w)
			static_perm &= ~0002;
		if(!x)
			static_perm &= ~0001;
	}
}

void main(int argc, char **argv) {
	char permstr[200];
	int permission;
	struct stat statbuf;
	int i = 0;
	
	if(argc != 3) {
		printf("usage : chmod [mode] [path] ");
		exit(1);
	}

	if(*argv[1] >= '0' && *argv[1] <= '7') {
		strcpy(permstr, argv[1]);
		sscanf(permstr, "%o", &permission);
		if(chmod(argv[2], (mode_t)permission) == -1)
			fprintf(stderr, "cannot change file mode\n");
	} 
	else {
		if(stat(argv[2], &statbuf) == -1) {
			fprintf(stderr, "Couldn't stat %s\n", argv[2]);
			exit(1);
		}
		while(*(argv[1]+i) != '\0') {
			if(*(argv[1]+i) == 'u')
				u = 1;
			else if(*(argv[1]+i) == 'g')
				g = 1;
			else if(*(argv[1]+i) == 'o')
				o = 1;
			else if(*(argv[1]+i) == 'a')
				a = 1;
			else if(*(argv[1]+i) == '+')
				p = 1;
			else if(*(argv[1]+i) == '-')
				m = 1;
			else if(*(argv[1]+i) == '=')
				e = 1;
			else if(*(argv[1]+i) == 'r')
				r = 1;
			else if(*(argv[1]+i) == 'w')
				w = 1;
			else if(*(argv[1]+i) == 'x')
				x = 1;
			else {
				fprintf(stderr, "wrong mode\n");
				exit(1);
			}
			++i;
		}
		static_perm |= (0777) & statbuf.st_mode;

		if(u)
			play_umode(argv[2]);
		if(g)
			play_gmode(argv[2]);
		if(o)
			play_omode(argv[2]);
		if(a) {
			play_umode(argv[2]);
			play_gmode(argv[2]);
			play_omode(argv[2]);
		}
		
		if(chmod(argv[2], (mode_t)static_perm) == -1)
			fprintf(stderr, "cannot change file mode\n");
	}
}
