#ifndef common_h
#define common_h

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/msg.h>
#include <sys/msg.h>

#define MAX_CLIENTS  10
#define PROJECT_ID 0x111
#define MSG_SIZE sizeof(msg_t)

int error(char * msg){
    perror(msg);
    exit(errno);
}

void delete_queue(int q_id){
    if (msgctl(q_id, IPC_RMID, NULL) < 0) 
        error("deleting queue");
}

typedef enum type_e {
    STOP = 1, 
    DISCONNECT = 2, 
    LIST = 3, 
    CONNECT = 4,
    INIT = 5,
    RESPONSE = 6,
} type_e;

typedef struct msg_t{
    long type;
    int sender_id;
    char message[MSGMAX];
} msg_t;

typedef struct queue_id{
    int q_id;
    key_t key;
    int aviable;
} queue_id;

#endif