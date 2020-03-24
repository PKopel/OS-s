#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ftw.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>

int MAXDEPTH = -1;

int get_option(char** args,int max,int i){
    if(i+1 >= max) return 22;
    if(strcmp(args[i],"-maxdepth") == 0) {
        MAXDEPTH = atoi(args[i+1]);
        if(MAXDEPTH >= 0){
        return 0;
    } 
    }
    return 22;
}

int search(char* starting_point,int level){
    if(MAXDEPTH >= 0 && level > MAXDEPTH) return 0;
    DIR* dir = opendir(starting_point);
    if(dir == NULL) return errno;
    struct dirent* current;
    struct stat* current_stat = 
        (struct stat*)calloc(1,sizeof(struct stat));
    char* next_point = 
        (char*)calloc(100,sizeof(char)),
        *abs_path = 
        (char*)calloc(100,sizeof(char));
    int result;
    while(dir != NULL){
        current = readdir(dir);
        if(strcmp(current->d_name, ".") != 0 
        && strcmp(current->d_name, "..") != 0){
            realpath(current->d_name,abs_path);
            if(lstat(abs_path,current_stat) == -1) return errno;
            if(current_stat->st_mode & S_IFDIR){
                strcpy(next_point,starting_point);
                strcat(next_point, "/");
                strcat(next_point, current->d_name);
                pid_t child_pid = vfork();
                if (child_pid == 0){
                    printf("%s\n",next_point);
                    printf("%d\n",getpid());
                    execlp("ls","ls","-l", next_point, NULL);
                    exit(0);
                } else {
                    int result;
                    waitpid(child_pid,&result, 0);
                    return result;
                }
                result = search(next_point,level+1);
                if(result != 0) return result;
            }
        }
    }
    free((void*)next_point);
    free((void*)abs_path);
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
    result = search(starting_point,0);
    return result;
}