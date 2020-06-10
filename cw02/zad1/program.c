#include <string.h>
#include <stdlib.h>
#include <sys/times.h>
#include "sysprog.h"
#include "libprog.h"

int error(char* msg){
    printf("%s\n",msg);
    return 22;
}

void write_time(char* name,int length, int size,struct tms start,struct tms end, FILE* file){
    double user = 
        (double)(end.tms_utime - start.tms_utime)/sysconf(_SC_CLK_TCK);
    double system = 
        (double)(end.tms_stime - start.tms_stime)/sysconf(_SC_CLK_TCK);
    fprintf(file, "%s ilość:\t%d\twielkość:\t%d\nuser:\t%f\tsystem:\t%f\t\n",
            name,length, size,user,system);
}

int main(int argc, char** argv){
    struct tms start, end;
    FILE* report = fopen("wynik.txt","a+");
    //length - number of records in file, size - size of one record
    int length, size, result;
    //check if command is "generate"
    if(argc == 5){
        if((length = atoi(argv[3])) > 0 
        && (size = atoi(argv[4])) > 0)
        result = copy_lib("/dev/urandom",argv[2],length,size);
        else result = error("wrong length or size, should be positive int");
    } 
    //check if command is "sort"
    else if(argc==6) {
        if((length = atoi(argv[3])) > 0 
        && (size = atoi(argv[4])) > 0){
            //check whitch implementation should be used
            if(strcmp("sys",argv[5])==0){
                times(&start);
                result = sort_sys(argv[2],length,size);
                times(&end);
                write_time("sortowanie sys:\n",length,size,start,end,report);
            } else if (strcmp("lib",argv[5])==0) {
                times(&start);
                result = sort_lib(argv[2],length,size);
                times(&end);
                write_time("sortowanie lib:\n",length,size,start,end,report);
            } else result = error("wrong last argument, should be sys or lib");
        } else result = error("wrong length or size, should be positive int");
    } 
    //check if command is "copy"
    else if(argc==7) {
        if((length = atoi(argv[4])) > 0 
        && (size = atoi(argv[5])) > 0){
            //check whitch implementation should be used
            if(strcmp("sys",argv[6])==0) {
                times(&start);
                result = copy_sys(argv[2],argv[3],length,size);
                times(&end);
                write_time("kopiowanie sys:\n",length,size,start,end,report);
            } else if (strcmp("lib",argv[6])==0) {
                times(&start);
                result = copy_lib(argv[2],argv[3],length,size);
                times(&end);
                write_time("kopiowanie lib:\n",length,size,start,end,report);
            } else result = error("wrong last argument, should be sys or lib");
        } else result = error("wrong length or size, should be positive int");
    } else result = error("wrong number of arguments\n");
    fclose(report);
    return result;
}