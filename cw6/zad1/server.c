#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "../common/common.h"

queue_id queues[MAX_CLIENTS + 1];
int counter = 1;


void handle_sigint(int signum){
    exit(EXIT_SUCCESS);
}

void send(msg_t* msg, int id){
     if (msgsnd(queues[id].q_id, msg, MSG_SIZE, 0) == -1) 
        error("server response");
}

void init(msg_t* msg){
    key_t client_key = msg->sender_id;
    int client_queue_id;
    if ((client_queue_id = msgget(client_key, 0)) == -1) 
            error("client queue");
    msg->type = RESPONSE;
    if (counter < MAX_CLIENTS){
        msg->sender_id = counter;
        queues[counter].q_id = client_queue_id;
        queues[counter].key = client_key;
    } else {
        msg->sender_id = -1;
    }
    send(msg,counter);
    while (queues[counter].q_id > 0 && counter < MAX_CLIENTS) counter++;
}

void stop(msg_t* msg){
    int i = msg->sender_id;
    queues[i].q_id = -1;
}

void list(msg_t* msg){
    int j = 0;
    for (int i = 0; i < MAX_CLIENTS; ++i){
        if (queues[i].q_id > 0 && i != msg->sender_id)
        msg->message[j++] = i;
        msg->message[j++] = queues[i].aviable;
    }
    msg->type = RESPONSE;
    send(msg,msg->sender_id);
}

void connect(msg_t * msg){
    int receiver_id = msg->message[0];
    if ()
    
    if (msgsnd(msg->cqkey, msg, MSG_SIZE, 0) == -1)     error("to one msgsnd");
}

void recognize_and_proceed(msg_t * msg){
    if (!msg) return;
    switch(msg->type){
        case INIT:
            log_in(msg);
            printf("Init client %d.\n", msg->sender_id);
        break;
        case STOP:
            stop(msg);
            printf("Client %d is gone.\n", msg->cqid);
        break;
        case LIST:
            list();
        break;
        case ECHO:
            printf("Server got >>%s<< from >>%d<<.\n", msg->mtext, msg->cqid);
            echo(msg);
        break;
        case TOALL:
            to_all(msg);
            printf("Server sent >>%s<< to all clients.\n", msg->mtext);
        break;
        case TOONE:
            to_one(msg);
            printf("Server sent >>%s<< to %d.\n", msg->message, msg->cqkey);
        break;
    }
}

int main(int argc, char ** argv){
    if ( atexit(delete_queue) < 0)                        error("atexit()");
    if ( signal(SIGINT, handle_sigint) == SIG_ERR)        error("signal()");

    struct msqid_ds current_state;

    char * path = getenv("HOME");
    if (!path)              error("getenv");

    sqkey = ftok(path, PROJECT_ID);
    if (sqkey == -1)          error("server ftok");

    q_desc = msgget(sqkey, IPC_CREAT | IPC_EXCL | 0666);
    if (q_desc < 0)         error("server msgget");

    msg_t msg;
    while(1){
        if (active == 0){
            if (msgctl(q_desc, IPC_STAT, &current_state) < 0)   error("msgctl");
            if (counter == 0)                    break;    // current_state.msg_qnum
        }
        if (msgrcv(q_desc, &msg, MSG_SIZE, 0, 0) < 0)           error("msgrcv");
        recognize_and_proceed(&msg);
    }

    return 0;
}