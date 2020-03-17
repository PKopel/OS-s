#include <string.h>
#include <stdlib.h>
#include "sysprog.h"
#include "libprog.h"
#include "generate.h"

int error(char* msg){
    printf("%s\n",msg);
    return 1;
}

int main(int argc, char** argv){
    //length - number of records in file, size - size of one record
    int length, size;
    //chack if command is generate
    if(argc == 4){
        if((length = atoi(argv[2])) > 0 
        && (size = atoi(argv[3])) > 0)
        return generate(argv[1],length,size);
        else return error("wrong length or size, should be positive int");
    } 
    //check if command is sort
    else if(argc==5) {
        if((length = atoi(argv[2])) > 0 
        && (size = atoi(argv[3])) > 0){
            //check whitch implementation should be used
            if(strcmp("sys",argv[4])==0) return sort_sys(argv[1],length,size);
            else if (strcmp("lib",argv[4])==0) return sort_lib(argv[1],length,size);
            else return error("wrong last argument, should be sys or lib");
        } else return error("wrong length or size, should be positive int");
    } 
    //check if command is copy
    else if(argc==6) {
        if((length = atoi(argv[3])) > 0 
        && (size = atoi(argv[4])) > 0){
            //check whitch implementation should be used
            if(strcmp("sys",argv[5])==0) return copy_sys(argv[1],argv[2],length,size);
            else if (strcmp("lib",argv[5])==0) return copy_lib(argv[1],argv[2],length,size);
            else return error("wrong last argument, should be sys or lib");
        } else return error("wrong length or size, should be positive int");
    } else return error("wrong number of arguments\n");
}
