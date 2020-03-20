#define _XOPEN_SOURCE 500
#include <memory.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int ROUNDM = 0;
int ROUNDA = 0;
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

int print_stat(struct stat* file,char* path){
    struct tm* mod_time, *acc_time;
    char* date = 
        (char*) calloc(20,sizeof(char)),
        *type = 
        (char*) calloc(10,sizeof(char));
    if(file->st_mode & S_IFDIR) strcpy(type,"dir");
    else if(file->st_mode & S_IFREG) strcpy(type,"file");
    else if(file->st_mode & S_IFIFO) strcpy(type,"fifo");
    else if(file->st_mode & S_IFLNK) strcpy(type,"slink");
    else if(file->st_mode & S_IFBLK)  strcpy(type,"block dev");
    else if(file->st_mode & S_IFCHR) strcpy(type,"char dev");
    else if(file->st_mode & S_IFSOCK) strcpy(type,"sock");
    mod_time = localtime(&file->st_mtime);
    acc_time = localtime(&file->st_atime);
    if(check_dates(file->st_mtime,file->st_atime)){
        printf("%s %ld %s %ld",
            path, file->st_nlink,
            type, file->st_size);
            strftime (date,20,"%c", acc_time);
            printf(" %s", date);
            strftime(date,20,"%c\n", mod_time);
            printf(" %s\n",date);
    }
    return 0;
}


int search(char* starting_point,int level){
    if(MAXDEPTH > 0 && level > MAXDEPTH) return 0;
    DIR* dir = opendir(starting_point);
    if(dir == NULL) return errno;
    struct dirent* current;
    struct stat* current_stat = NULL;
    char* next_point = 
        (char*)calloc(100,sizeof(char)),
        abs_path = 
        (char*)calloc(100,sizeof(char));
    int result;
    while(dir != NULL){
        current = readdir(dir);
        if(strcmp(current->d_name, ".") != 0 
        && strcmp(current->d_name, "..") != 0){
            realpath(current->d_name,abs_path);
            if(lstat(abs_path,current_stat) == -1) return errno;
            print_stat(current_stat,abs_path);
            if(current_stat->st_mode & S_IFDIR){
                strcpy(next_point,starting_point);
                strcat(next_point, "/");
                strcat(next_point, current->d_name);
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