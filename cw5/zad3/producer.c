#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

int error(char* msg){
    perror(msg);
    exit(errno);
}

int send(char* file_name, char* fifo_name, int buff_size){
    char* buff = 
        (char*)calloc(buff_size,sizeof(char)),
        *marked_buff = 
        (char*)calloc(buff_size+8,sizeof(char));
    int fifo = open(fifo_name,O_WRONLY),
        file = open(file_name,O_RDONLY);
    if( fifo < 0 || file < 0) error("open");
    int chars_read, pid = getpid();
    srand(time(NULL));
    while((chars_read = read(file,buff,buff_size)) > 0){
        sprintf(marked_buff,"#%d#%s",pid,buff);
        write(fifo,marked_buff,chars_read+8);
        sleep(rand()%2+1);
    }
    close(fifo);
    close(file);
    free((void*)buff);
    free((void*)marked_buff);
    return 0;
}

int main(int argc, char** argv){
    if(argc != 4) return 22;
    int buff_size = atoi(argv[3]);
    if(buff_size > 0) return send(argv[2],argv[1],buff_size);
    else error("buff size");
}