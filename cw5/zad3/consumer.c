#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int receive(int file, int fifo, int buff_size){
    char* buff = 
        (char*)calloc(buff_size,sizeof(char));
    int chars_read;
    while((chars_read = read(fifo,buff,buff_size)) > 0){
        write(file,buff,chars_read);
    }
    close(file);
    close(fifo);
    free((void*)buff);
    return 0;
}

int main(int argc, char** argv){
    if(argc != 4) return 22;
    int fifo = open(argv[1],O_WRONLY);
    int file = open(argv[2],O_RDONLY);
    int buff_size = atoi(argv[3]);
    return receive(file,fifo,buff_size);
}