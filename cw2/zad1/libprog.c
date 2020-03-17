#include "libprog.h"
//length means number of records in a file, size is size of one record

int copy_lib(char* file1, char* file2, int length, int size){
    char* data = (char*)calloc(size,sizeof(char));
    int result;
    FILE* old_file = fopen(file1,"r");
    FILE* new_file = fopen(file2,"w");
    for(int i = 0; i < length; i++){
         result = fread((void*)data,sizeof(char),size,old_file);
         if(result != size){
             printf("error while reading data\n");
             fclose(old_file);
             fclose(new_file);
             return 5;
         };
         result = fwrite((void*)data,sizeof(char),size,new_file);
         if(result != size){
             printf("error while writing data\n");
             fclose(old_file);
             fclose(new_file);
             return 5;
         };
    }
    fclose(old_file);
    fclose(new_file);
    free((void*)data);
    return 0;
}

int sort_lib(char* file, int length, int size){
    return 1;
}

