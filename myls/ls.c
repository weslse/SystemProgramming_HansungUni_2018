#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void setOptionFlag(int, char**, char**, int*, int*);
char type(mode_t);
char *perm(mode_t);
void printStat(char*, char*, struct stat*);

int main(int argc, char** argv)
{
    DIR *dp;
    char *dir;
    struct stat st;
    struct dirent *d;
    char path[BUFSIZ+1];
    int lOptionFlag = 0;
    int aOptionFlag = 0;
    
    setOptionFlag(argc, argv, &dir, &lOptionFlag, &aOptionFlag);

    if((dp = opendir(dir)) == NULL)
        perror(dir);
    //printf("Success Open\n");
    //printf("%d %d %s\n", lOptionFlag, aOptionFlag, dir);
    
    while((d = readdir(dp)) != NULL) {
	if(!aOptionFlag && d->d_name[0] == '.')
	    continue;
	sprintf(path, "%s/%s", dir, d->d_name);

        if(lOptionFlag){
	    if(lstat(path, &st) < 0)
                perror(path);
            printStat(path, d->d_name, &st); 
	    putchar('\n');
	}
	else
	    printf("%s ", d->d_name);
    }
    putchar('\n');
    closedir(dp);
    return 0; 
}

void setOptionFlag(int argc, char** argv, char** dir, 
        int* lOptionFlag, int* aOptionFlag)
{
    if(argc==1)
        *dir = ".";

    if(argc==2){
        if(argv[1][0] == '-') {
            *dir = ".";
            if(strlen(argv[1]) == 3) {
                *lOptionFlag = 1;
                *aOptionFlag = 1;
		//printf("setDouble\n");
            }
            if(argv[1][1] == 'l'){
		//printf("loptionON\n");
                *lOptionFlag = 1;
	    }
            if(argv[1][1] == 'a'){
		//printf("aoptionON\n");
                *aOptionFlag = 1; 
	    } 
        }
        else{
	    //printf("argc2 dir set\n");
            *dir = argv[1];
	}
    }

    if(argc == 3){
        *dir = argv[2];
        if(strlen(argv[1]) == 3) {
            *lOptionFlag = 1;
            *aOptionFlag = 1;
        }
        if(argv[1][1] == 'l'){
	    //printf("loptionON\n");
            *lOptionFlag = 1;
	}
        if(argv[1][1] == 'a'){
	    //printf("aoptionON\n");
            *aOptionFlag = 1; 
	}
    }
}

void printStat(char* pathName, char* file, struct stat* st) {
    printf("%5d ", st->st_blocks);
    printf("%c%s ", type(st->st_mode), perm(st->st_mode));
    printf("%3d ", st->st_nlink);
    printf("%s %s ", getpwuid(st->st_uid)->pw_name, 
                    getgrgid(st->st_gid)->gr_name);
    printf("%9d ", st->st_size);
    printf("%.12s ", ctime(&st->st_mtime)+4);
    printf("%s", file);
}

char type(mode_t mode) {
    if(S_ISREG(mode))
        return('-');
    if(S_ISDIR(mode))
        return('d');
    if(S_ISCHR(mode))
        return('c');
    if(S_ISBLK(mode))
        return('b');
    if(S_ISLNK(mode))
        return('l');
    if(S_ISFIFO(mode))
        return('p');
    if(S_ISSOCK(mode))
        return('s');
}

char* perm(mode_t mode) {
    int i;
    static char perms[10] = "---------";

    for(i = 0; i < 3; i++) {
        if(mode & (S_IREAD >> i*3))
            perms[i*3] = 'r';
        if(mode & (S_IWRITE >> i*3))
            perms[i*3+1] = 'w';
        if(mode & (S_IEXEC >> i*3))
            perms[i*3+2] = 'x';
    }
    return (perms);
}
