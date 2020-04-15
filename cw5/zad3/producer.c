#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int send(int file, int fifo, int buff_size){
    char* buff = 
        (char*)calloc(buff_size,sizeof(char)),
        *marked_buff = 
        (char*)calloc(buff_size+8,sizeof(char));

    int chars_read;
    while((chars_read = read(file,buff,buff_size)) > 0){
        sleep(1);
        sscanf(marked_buff,"#%d#%s\n",getpid(),buff);
        write(fifo,marked_buff,chars_read+8);
    }
    close(fifo);
    close(file);
    free((void*)buff);
    free((void*)marked_buff);
    return 0;
}

int main(int argc, char** argv){
    if(argc != 4) return 22;
    int fifo = open(argv[1],O_WRONLY);
    int file = open(argv[2],O_RDONLY);
    int buff_size = atoi(argv[3]);
    return send(file,fifo,buff_size);
}