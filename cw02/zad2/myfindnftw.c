#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>
#include <string.h>

int ROUNDM = 1;
int ROUNDA = 1;
int MTIME = -1;
int ATIME = -1;
int MAXDEPTH = -1;

int get_option(char** args,int max,int i){
    if(i+1 >= max) return 22;
   if(strcmp(args[i],"-mtime") == 0){
        if(args[i+1][0] == '-') ROUNDM = -1;
        if(args[i+1][0] == '+') ROUNDM = 1;
        MTIME = atoi(args[i+1]); return 0;
    } 
    if(strcmp(args[i],"-atime") == 0) {
        if(args[i+1][0] == '-') ROUNDA = -1;
        if(args[i+1][0] == '+') ROUNDA = 1;
        ATIME = atoi(args[i+1]); return 0;
    } 
    if(strcmp(args[i],"-maxdepth") == 0) {
        if((MAXDEPTH = atoi(args[i+1]) >= 0)) return 0;
    }
    return 22;
}

int check_dates(time_t mtime, time_t atime){
    int result = 0;
    time_t now = time(NULL);
    time_t then_m = now - MTIME*24*60*60*ROUNDM;
    time_t then_a = now - ATIME*24*60*60*ROUNDA;
    if(ROUNDM < 0) result = (mtime > then_m);
    else result = (mtime < then_m);
    if (ROUNDA < 0) result = result && (atime > then_a);
    else result = result && (atime < then_a);
    return result;
}


int print_stat(char *path, const struct stat *sb, int typeflag, struct FTW *tw_buf){
    if (MAXDEPTH > 0 && tw_buf->level > MAXDEPTH) return 0; 
    struct tm* mod_time, *acc_time;
    char *date = 
        (char*) calloc(20,sizeof(char)),
        *type = 
        (char*) calloc(10,sizeof(char));
    if(sb->st_mode & S_IFDIR) strcpy(type,"dir");
    else if(sb->st_mode & S_IFREG) strcpy(type,"file");
    else if(sb->st_mode & S_IFIFO) strcpy(type,"fifo");
    else if(sb->st_mode & S_IFLNK) strcpy(type,"slink");
    else if(sb->st_mode & S_IFBLK)  strcpy(type,"block dev");
    else if(sb->st_mode & S_IFCHR) strcpy(type,"char dev");
    else if(sb->st_mode & S_IFSOCK) strcpy(type,"sock");

    mod_time = localtime(&sb->st_mtime);
    acc_time = localtime(&sb->st_atime);
    if(check_dates(sb->st_mtime,sb->st_atime)){
        printf("%s %ld %s %ld",
            path, sb->st_nlink,
            type, sb->st_size);
            strftime (date,20,"%c", acc_time);
            printf(" %s", date);
            strftime(date,20,"%c\n", mod_time);
            printf(" %s\n",date);
    }
    return 0;
}

int main(int argc, char** argv){
    if(argc <=1) return 22;
    int i = 1, result;
    char* starting_point;
    while(i < argc){
        if(argv[i][0] == '-') {
            result = get_option(argv,argc,i);
            if (result != 0) return result;
            i+=2;
        } else {
            starting_point = argv[i++];
        }
    }
    char* path = 
        (char*)calloc(100,sizeof(char));   
    realpath(starting_point, path);
    return nftw(path, print_stat, 10, FTW_PHYS | FTW_DEPTH);
}