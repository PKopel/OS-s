#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ftw.h>
#include <string.h>
#include <unistd.h>

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

int run_ls(char *path, const struct stat *sb, int typeflag, struct FTW *tw_buf){
    if (MAXDEPTH > 0 && tw_buf->level > MAXDEPTH) return 0; 
    if (sb->st_mode & S_IFDIR){
        pid_t child_pid = vfork();
        if (child_pid == 0){
            printf("%s\n",path);
            printf("%d\n",getpid());
            execlp("ls","ls","-l", path);
            exit(0);
        } else {
            int result;
            waitpid(child_pid,&result, 0);
            return result;
        }
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
    return nftw(path, run_ls, 10, FTW_PHYS | FTW_DEPTH);
}