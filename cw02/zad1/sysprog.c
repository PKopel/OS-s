#include "sysprog.h"
//length means number of records in a file, size is size of one record

char* pivot;
int err_sys = 0;

int error_sys(char* msg, int file1, int file2){
    close(file1);
    close(file2);
    err_sys = 1;
    write(1,msg,strlen(msg));
    return 5;
}

int gt_eq_sys (char* block) { 
  return strcmp(pivot,block) < 0; 
} 

int sm_sys (char* block) { 
  return strcmp(pivot,block) >= 0; 
} 

int tr_sys (char* block){
    return 1;
}

int seek_file_sys(int file_to_reset,int file_to_leave,int off,int mode){
    long result = lseek(file_to_reset,off,mode);
    if(result < 0){
        free((void*)pivot);
        return error_sys(
        "error while reseting file pointer\n",
        file_to_reset,file_to_leave);
    }
    return 0;
}

int write_file_sys(int file_to_write,int file_to_leave,char* block,int size){
    int result = 
        write(file_to_write,block,size);
    if(result != size){
        free((void*)block);
        return error_sys(
            "error while writing data\n",
            file_to_write,file_to_leave);
    };
    return 0;
}

int read_file_sys(int file_to_read,int file_to_leave,char* block,int size){
    int result = 
        read(file_to_read,block,size);
    if(result != size){
        free((void*)block);
        return error_sys(
            "error while reading data\n",
            file_to_read,file_to_leave);
    };
    return 0;
}

int copy_blocks_sys(int from_file, int to_file,
               int length, int size,
               int (*compare)(char*)){
    int result, count = 0;
    char* block = 
        (char*)calloc(size,sizeof(char));
    for(int i = 0; i< length; i++){
        result = 
            read_file_sys(from_file,to_file,block,size);
        if(result != 0) return result;
        if(compare(block)){
            result = 
                write_file_sys(to_file,from_file,block,size);
            if(result != 0) return result;
            count++;
        }
    }
    free((void*)block);
    return count;
}

int copy_sys(char* file1, char* file2, int length, int size){
    int result;
    int old_file = open(file1,O_RDONLY);
    int new_file = open(file2,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if(old_file == -1 || new_file == -1){
        return error_sys(
            "error while opening files\n",
            old_file,new_file);
    }
    result = copy_blocks_sys(old_file,new_file,length,size,tr_sys);
    if(err_sys == 0){
        close(old_file);
        close(new_file);
        return 0;
    }
    return result;
}

int sort_file_sys(int old_file,int tmp_file, int length, int size){
    int result;
    if(length <= 1) {
        result = seek_file_sys(old_file,tmp_file,length*size,SEEK_CUR);
        if (result != 0) return result;
        result = seek_file_sys(tmp_file,old_file,length*size,SEEK_CUR);
        return result;
    }
    pivot = (char*)calloc(size,sizeof(char));
    int smaller = 0, bigger = 0;
    result = 
        read_file_sys(old_file,tmp_file,pivot,size);
    if(result != 0) return result;
    //copy blocks smaller than pivot to tmp (goes throught whole part)
    result = 
        copy_blocks_sys(old_file,tmp_file,length-1,size,sm_sys);
    if(err_sys != 0) return result;
    else smaller = result;
    //reset old_file to block after pivot
    result = 
        seek_file_sys(old_file,tmp_file,-(length-1)*size,SEEK_CUR);
    if(result != 0)return result;
    //write pivot to tmp
    result = 
        write_file_sys(tmp_file,old_file,pivot,size);
    if(result != 0) return result;
    //copy blocks greater than pivot to tmp (whole part again)
    result = 
        copy_blocks_sys(old_file,tmp_file,length-1,size,gt_eq_sys);
    if(err_sys != 0) return result;
    else bigger = result;
    //reset tmp_file to start of current part
    result = 
        seek_file_sys(tmp_file,old_file,-length*size,SEEK_CUR);
    if(result != 0)return result;
    //reset old_file to start of current part
    result = 
        seek_file_sys(old_file,tmp_file,-length*size,SEEK_CUR);
    if(result != 0)return result;
    free((void*)pivot);
    //copy "sorted" part from tmp to old
    result =
        copy_blocks_sys(tmp_file,old_file,length,size,tr_sys);
    if(err_sys != 0) return result;
    //reset tmp and old again
    result = 
        seek_file_sys(tmp_file,old_file,-length*size,SEEK_CUR);
    if(result != 0)return result;
    result = 
        seek_file_sys(old_file,tmp_file,-length*size,SEEK_CUR);
    if(result != 0)return result;
    //sort smaller blocks
    result =
        sort_file_sys(old_file,tmp_file,smaller,size);
    if(result != 0) return result;
    else smaller = result;
    //ommit pivot in both files
    result = 
        seek_file_sys(tmp_file,old_file,size,SEEK_CUR);
    if(result != 0)return result;
    result = 
        seek_file_sys(old_file,tmp_file,size,SEEK_CUR);
    if(result != 0)return result;
    //sort greater blocks
    result =
        sort_file_sys(old_file,tmp_file,bigger,size);
    if(result != 0) return result;
    return 0;
}

int sort_sys(char* file, int length, int size){
    char* tmp_name = 
        (char*)calloc(strlen(file)+4,sizeof(char));
    strcpy(tmp_name,"tmp_");
    strcat(tmp_name,file);
    int old_file = open(file,O_RDWR);
    int tmp_file = open(tmp_name,O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
    if(old_file == -1 || tmp_file == -1) 
        return error_sys(
            "error while opening files"
            ,old_file,tmp_file);
    int result = 
        sort_file_sys(old_file,tmp_file,length,size);
    if(result != 0) return result;
    close(old_file);
    close(tmp_file);
    unlink(tmp_name);
    free((void*)tmp_name);
    return result;
}