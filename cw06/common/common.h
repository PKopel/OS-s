#ifndef common_h
#define common_h

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define MAX_CLIENTS  10
#define MSG_MAX 256
#define MSG_SIZE sizeof(msg_t)

const char* server_name = "/serv_cw6";

key_t server_key(){
    return ftok("/tmp",0x112);
}

int error(char * msg){
    perror(msg);
    exit(errno);
}

typedef enum type_e {
    STOP = 1, 
    DISCONNECT = 2, 
    LIST = 3, 
    CONNECT = 4,
    INIT = 5,
    RESPONSE = 6,
    FINISH = 7
} type_e;

typedef struct msg_t{
    long type;
    int sender_id;
    char message[MSG_MAX];
} msg_t;

typedef struct queue_id{
    int q_id;
    key_t key;
    int aviable;
} queue_id;

queue_id init_queue_id(){
    queue_id new_queue;
    new_queue.aviable = 1;
    new_queue.q_id = -1;
    return new_queue;
}

#endif