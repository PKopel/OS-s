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

int gt_eq (char* block) { 
  return strcmp(pivot,block) < 0; 
} 

int sm (char* block) { 
  return strcmp(pivot,block) >= 0; 
} 

int tr (char* block){
    return 1;
}

int seek_file(FILE* file_to_reset,FILE* file_to_leave,int off,int mode){
    int result = fseek(file_to_reset,off,mode);
    if(result != 0){
        free((void*)pivot);
        return error_lib(
        "error while reseting file pointer",
        file_to_reset,file_to_leave);
    }
    return 0;
}

int write_file(FILE* file_to_write,FILE* file_to_leave,char* block,int size){
    int result = 
        fwrite((void*)block,sizeof(char),size,file_to_write);
    if(result != size){
        free((void*)block);
        return error_lib(
            "error while reading data",
            file_to_write,file_to_leave);
    }
    return 0;
}

int read_file(FILE* file_to_read,FILE* file_to_leave,char* block,int size){
    int result = 
        fread((void*)block,sizeof(char),size,file_to_read);
    if(result != size){
        free((void*)block);
        return error_lib(
            "error while reading data",
            file_to_read,file_to_leave);
    }
    return 0;
}

int copy_blocks(FILE* from_file, FILE* to_file,
               int length, int size,
               int (*compare)(char*)){
    int result, count = 0;
    char* block = 
        (char*)calloc(size,sizeof(char));
    for (int i = 0; i < length; i++){
        result = 
            read_file(from_file,to_file,block,size);
        if (result != 0) return result;
        if(compare(block)){
            result = 
                write_file(to_file,from_file,block,size);
            if (result != 0) return result;
            count++;
        }
    }
    free((void*)block);
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
    result = copy_blocks(old_file,new_file,length,size,tr);
    if(err == 0){
        fclose(old_file);
        fclose(new_file);
        return 0;
    }
    return result;
}

int sort_file(FILE* old_file, FILE* tmp_file, int length, int size){
    int result;
    if(length <= 1) {
        result = seek_file(old_file,tmp_file,length*size,1);
        if (result != 0) return result;
        result = seek_file(tmp_file,old_file,length*size,1);
        return result;
    }
    pivot = (char*)calloc(size,sizeof(char));
    int smaller = 0, bigger = 0;
    result = 
        read_file(old_file,tmp_file,pivot,size);
    if(result != 0) return result;
    //copy blocks smaller than pivot to tmp (goes throught whole part)
    result = 
        copy_blocks(old_file,tmp_file,length-1,size,sm);
    if(err != 0) return result;
    else smaller = result;
    //reset old_file to block after pivot
    result = 
        seek_file(old_file,tmp_file,-(length-1)*size,1);
    if(result != 0)return result;
    //write pivot to tmp
    result = 
        write_file(tmp_file,old_file,pivot,size);
    if(result != 0) return result;
    //copy blocks greater than pivot to tmp (whole part again)
    result = 
        copy_blocks(old_file,tmp_file,length-1,size,gt_eq);
    if(err != 0) return result;
    else bigger = result;
    //reset tmp_file to start of current part
    result = 
        seek_file(tmp_file,old_file,-length*size,1);
    if(result != 0)return result;
    //reset old_file to start of current part
    result = 
        seek_file(old_file,tmp_file,-length*size,1);
    if(result != 0)return result;
    free((void*)pivot);
    //copy "sorted" part from tmp to old
    result =
        copy_blocks(tmp_file,old_file,length,size,tr);
    if(err != 0) return result;
    //reset tmp and old again
    result = 
        seek_file(tmp_file,old_file,-length*size,1);
    if(result != 0)return result;
    result = 
        seek_file(old_file,tmp_file,-length*size,1);
    if(result != 0)return result;
    //sort smaller blocks
    result =
        sort_file(old_file,tmp_file,smaller,size);
    if(result != 0) return result;
    else smaller = result;
    //ommit pivot in both files
    result = 
        seek_file(tmp_file,old_file,size,1);
    if(result != 0)return result;
    result = 
        seek_file(old_file,tmp_file,size,1);
    if(result != 0)return result;
    //sort greater blocks
    result =
        sort_file(old_file,tmp_file,bigger,size);
    if(result != 0) return result;
    return 0;
}

int sort_lib(char* file, int length, int size){
    char* tmp_name = 
        (char*)calloc(strlen(file)+4,sizeof(char));
    strcpy(tmp_name,"tmp_");
    strcat(tmp_name,file);
    FILE* old_file = fopen(file,"r+");
    FILE* tmp_file = freopen(tmp_name,"r+",fopen(tmp_name,"w"));
    if(old_file == NULL || tmp_file == NULL) 
        return error_lib(
            "error while opening files"
            ,old_file,tmp_file);
    int result = 
        sort_file(old_file,tmp_file,length,size);
    if(result != 0) return result;
    fclose(old_file);
    fclose(tmp_file);
    remove(tmp_name);
    free((void*)tmp_name);
    return result;
}