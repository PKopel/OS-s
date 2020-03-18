#include "libprog.h"
//length means number of records in a file, size is size of one record

char* pivot;
int err = 0;

int error_lib(char* msg, FILE* file1,FILE* file2){
    fclose(file1);
    fclose(file2);
    err = 1;
    printf("%s\n",msg);
    return 5;
}

int gt_eq (char* line) 
{ 
  return strcmp(pivot,line) >=0; 
} 

int sm (char* line) 
{ 
  return strcmp(pivot,line) < 0; 
} 

int tr (char* line){
    return 1;
}

int reset_file(FILE* file_to_reset,FILE* file_to_leave,int off,int mode){
    int result = fseek(file_to_reset,off,mode);
    if(result != 0){
        free((void*)pivot);
        return error_lib(
        "error while reseting file pointer",
        file_to_reset,file_to_leave);
    }
}

int write_file(FILE* file_to_write,FILE* file_to_leave,char* line,int size){
    int result = 
        fwrite((void*)line,sizeof(char),size,file_to_write);
    if(result != size){
        free((void*)line);
        return error_lib(
            "error while reading data",
            file_to_write,file_to_leave);
    }
    return 0;
}

int read_file(FILE* file_to_read,FILE* file_to_leave,char* line,int size){
    int result = 
        fread((void*)line,sizeof(char),size,file_to_read);
    if(result != size){
        free((void*)line);
        return error_lib(
            "error while reading data",
            file_to_read,file_to_leave);
    }
    return 0;
}

int copy_lines(FILE* old_file, FILE* new_file,
               int length, int size,
               int (*compare)(char*)){
    int result, count = 0;
    char* line = 
        (char*)calloc(size,sizeof(char));
    for (int i = 0; i < length-1; i++){
        result = 
            read_file(old_file,new_file,line,size);
        if (result != 0) return result;
        if(compare(line)){
            result = 
                write_file(new_file,old_file,line,size);
            if (result != 0) return result;
            count++;
        }
    }
    free((void*)line);
    return count;
}

int copy_lib(char* file1, char* file2, int length, int size){
    int result;
    FILE* old_file = fopen(file1,"r");
    FILE* new_file = fopen(file2,"w");
    if(old_file == NULL || new_file == NULL)
        return error_lib(
            "error while opening files",
            old_file,new_file);
    result = copy_lines(old_file,new_file,length,size,tr);
    if(err == 0){
        fclose(old_file);
        fclose(new_file);
    }
    return result;
}

int sort_lib(char* file, int length, int size){
    char* tmp_name = 
        (char*)calloc(strlen(file)+4,sizeof(char));
    strcpy(tmp_name,"tmp_");
    strcat(tmp_name,file);
    FILE* old_file = fopen(file,"w+");
    FILE* tmp_file = fopen(tmp_name,"w+");
    free((void*)tmp_name);
    if(old_file == NULL || tmp_file == NULL) 
        return error_lib(
            "error while opening files"
            ,old_file,tmp_file);
    pivot = (char*)calloc(size,sizeof(char));
    int smaller = 0, bigger = 0;
    int result = 
        read_file(old_file,tmp_file,pivot,size);
    if(result != 0) return result;
    result = 
        copy_lines(old_file,tmp_file,length,size,gt_eq);
    if(err != 0) return result;
    else bigger = result;
    result = 
        reset_file(old_file,tmp_file,-(length-1)*size,1);
    if(result != 0)return result;
    result = 
        write_file(tmp_file,old_file,pivot,size);
    if(result != 0) return result;
    result = 
        copy_lines(old_file,tmp_file,length,size,sm);
    if(err != 0) return result;
    else smaller = result;
    result = 
        reset_file(old_file,tmp_file,-(length-1)*size,1);
    if(result != 0)return result;
    return result;
}

