#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int error(char* msg){
    perror(msg);
    exit(errno);
}

int main(void){
    srand(time(NULL));
    char* file_name = 
        (char*)calloc(20,sizeof(char)),
        * fifo_name = 
        (char*)calloc(20,sizeof(char));
    int current = rand()%100;
    sprintf(fifo_name,"%s_%02d","fifo",current);
    mkfifo(fifo_name,0666);
    pid_t pid = fork();
    char num[2];
    if (pid == 0){
        sprintf(file_name,"%s_%02d","out",current);
        sprintf(num,"%d",rand()%10+10);
        if(execl("consumer.o","consumer.o",fifo_name,file_name,num, NULL) < 0) error("exec consumer");
        exit(0);
    } else if(pid > 0){
        for (int i = 0; i < 5; i++){
            pid = fork();
            if(pid == 0){
                sprintf(file_name,"%s_%02d","in",i);
                sprintf(num,"%d",rand()%10+5);
                if(execl("producer.o","producer.o",fifo_name,file_name,num, NULL) < 0) error("exec producer");
                exit(0);
            } else if(pid < 0) error("fork producer");
        }
    } else error("fork consumer");
    free((void*)file_name);
    free((void*)fifo_name);
    return 0;
}