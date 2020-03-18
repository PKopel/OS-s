#include "sysprog.h"
#include <stdio.h>
//length means number of records in a file, size is size of one record

int error_sys(char* msg, int file1, int file2){
    close(file1);
    close(file2);
    printf("%s\n",msg);
    return 5;
}

int copy_sys(char* file1, char* file2, int length, int size){
    int result;
    int old_file = open(file1,O_RDONLY);
    int new_file = open(file2,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if(old_file == -1 || new_file == -1){
        return error_sys(
            "error while opening files",
            old_file,new_file);
    }
    char* data = 
        (char*)calloc(size,sizeof(char));
    for(int i = 0; i< length; i++){
         result = read(old_file,data,size);
         if(result != size){
             free((void*)data);
             return error_sys(
                "error while reading data",
                old_file,new_file);
         };
         result = write(new_file,data,result);
         if(result != size){
             free((void*)data);
             return error_sys(
                "error while writing data",
                old_file,new_file);
         };
    }
    close(old_file);
    close(new_file);
    free((void*)data);
    return 0;
}

int sort_sys(char* file, int length, int size){
    return 1;
}
