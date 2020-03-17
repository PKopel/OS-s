#include "generate.h"

int generate(char* filename, int length, int size){
    char* data = (char*)calloc(size,sizeof(char));
    int result;
    FILE* rand = fopen("/dev/urandom","r");
    FILE* new_file = fopen(filename,"w");
    for(int i = 0; i< length; i++){
         result = fread(data,sizeof(char),size,rand);
         if(result < size){
             printf("error while generating random data\n");
             return 1;
         };
         result = fwrite(data,sizeof(char),size,new_file);
         if(result < size){
             printf("error while writing data to file\n");
             return 1;
         };
    }
    fclose(rand);
    fclose(new_file);
    free((void*)data);
}