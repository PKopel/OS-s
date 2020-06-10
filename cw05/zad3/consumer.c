#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int error(char* msg){
    perror(msg);
    exit(errno);
}

int receive(FILE* file, int fifo, int buff_size){
    char* buff = 
        (char*)calloc(buff_size,sizeof(char));
    int chars_read;
    while((chars_read = read(fifo,buff,buff_size)) > 0){
        fprintf(file,"%s",buff);
    }
    fclose(file);
    close(fifo);
    free((void*)buff);
    return 0;
}

int main(int argc, char** argv){
    if(argc != 4) return 22;
    int fifo = open(argv[1],O_RDONLY);
    FILE* file = fopen(argv[2],"w");
    if(file == NULL || fifo < 0) error("open");
    int buff_size = atoi(argv[3]);
    if(buff_size > 0) return receive(file,fifo,buff_size);
    else error("buff size");
}