#include "sysprog.h"
#include <stdio.h>
//length means number of records in a file, size is size of one record

int copy_sys(char* file1, char* file2, int length, int size){
    char* data = (char*)calloc(size,sizeof(char));
    int result;
    int old_file = open(file1,O_RDONLY);
    int new_file = open(file2,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    for(int i = 0; i< length; i++){
         result = read(old_file,data,size);
         if(result != size){
             printf("error while reading data\n");
             close(old_file);
             close(new_file);
             return 5;
         };
         result = write(new_file,data,result);
         if(result != size){
             printf("error while writing data\n");
             close(old_file);
             close(new_file);
             return 5;
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
