#include <string.h>
#include <stdlib.h>
#include "sysprog.h"
#include "libprog.h"

int error(char* msg){
    printf("%s\n",msg);
    return 22;
}

int main(int argc, char** argv){
    //length - number of records in file, size - size of one record
    int length, size;
    //chack if command is generate
    if(argc == 5){
        if((length = atoi(argv[3])) > 0 
        && (size = atoi(argv[4])) > 0)
        return copy_lib("/dev/urandom",argv[2],length,size);
        else return error("wrong length or size, should be positive int");
    } 
    //check if command is sort
    else if(argc==6) {
        if((length = atoi(argv[3])) > 0 
        && (size = atoi(argv[4])) > 0){
            //check whitch implementation should be used
            if(strcmp("sys",argv[5])==0) return sort_sys(argv[2],length,size);
            else if (strcmp("lib",argv[5])==0) return sort_lib(argv[2],length,size);
            else return error("wrong last argument, should be sys or lib");
        } else return error("wrong length or size, should be positive int");
    } 
    //check if command is copy
    else if(argc==7) {
        if((length = atoi(argv[4])) > 0 
        && (size = atoi(argv[5])) > 0){
            //check whitch implementation should be used
            if(strcmp("sys",argv[6])==0) return copy_sys(argv[2],argv[3],length,size);
            else if (strcmp("lib",argv[6])==0) return copy_lib(argv[2],argv[3],length,size);
            else return error("wrong last argument, should be sys or lib");
        } else return error("wrong length or size, should be positive int");
    } else return error("wrong number of arguments\n");
}
