#include "../common/common.h"

queue_id queues[MAX_CLIENTS + 1];
int starting = 1;
int counter = 0;
int current = 1;

void send(msg_t* msg, int id){
     if (msgsnd(queues[id].q_id, msg, MSG_SIZE, 0) == -1) 
        error("server send");
}

void delete_queue(int q_id){
    if (msgctl(q_id, IPC_RMID, NULL) < 0) 
        error("deleting queue");
}

void handle_sigint(int signum){
    msg_t msg;
    msg.type = FINISH;
    msg.sender_id = 0;
    for(int i = 1; i< MAX_CLIENTS + 1; i++){
        if (queues[i].q_id != -1){
            send(&msg,i);
        }
    }
    while (counter > 0){
        if (msgrcv(queues[0].q_id, &msg, MSG_SIZE, STOP, 0) < 0)           
            error("server receive");
        else counter--;
    }
    exit(EXIT_SUCCESS);
}

void init(msg_t* msg){
    key_t client_key = msg->sender_id;
    int client_queue_id;
    if ((client_queue_id = msgget(client_key, 0)) == -1) 
            error("client queue");
    msg->type = RESPONSE;
    if (counter < MAX_CLIENTS){
        printf("new client #%d\n", current);
        msg->sender_id = current;
        queues[current].q_id = client_queue_id;
        queues[current].key = client_key;
        counter++;
        starting = 0;
    } else {
        msg->sender_id = -1;
    }
    send(msg,current);
    while (queues[current].q_id > 0 && current < MAX_CLIENTS) current++;
}

void stop(msg_t* msg){
    int i = msg->sender_id;
    if(msg->message[0]){
        int connected_id = msg->message[1];
        queues[connected_id].aviable = 1;
        msg->type = DISCONNECT;
        msg->sender_id = 0;
        send(msg,connected_id);
    }
    counter--;
    current = i;
    queues[i].q_id = -1;
}

void list(msg_t* msg){
    int j = 1;
    msg->message[0] = counter;
    for (int i = 1; i < MAX_CLIENTS; ++i){
        if (queues[i].q_id > 0 && i != msg->sender_id)
        msg->message[j++] = i;
        msg->message[j++] = queues[i].aviable;
    }
    msg->type = RESPONSE;
    msg->sender_id = 0;
    send(msg,msg->sender_id);
}

void connect(msg_t * msg){
    int receiver_id = msg->message[0];
    int sender_id = msg->sender_id;
    if (queues[receiver_id].aviable && queues[sender_id].aviable){
        queues[sender_id].aviable = 0;
        queues[receiver_id].aviable = 0;
        msg->type = CONNECT;
        msg->sender_id = queues[receiver_id].key;
        msg->message[0] = sender_id;
        send(msg,sender_id);
        msg->sender_id = queues[sender_id].key;
        send(msg,receiver_id);
    } else {
        msg->type = RESPONSE;
        msg->sender_id = -1;
        send(msg,sender_id);
    }
}

void disconnect(msg_t* msg){
    int receiver_id = msg->message[0];
    int sender_id = msg->sender_id;
    queues[sender_id].aviable = 1;
    queues[receiver_id].aviable = 1;
}

void resolve(msg_t * msg){
    if (!msg) return;
    switch(msg->type){
        case INIT:
        printf("INIT\n");
            init(msg);
        break;
        case STOP:
        printf("STOP\n");
            stop(msg);
        break;
        case LIST:
        printf("LIST\n");
            list(msg);
        break;
        case CONNECT:
        printf("CONNECT\n");
            connect(msg);
        break;
        case DISCONNECT:
        printf("DISCONNECT\n");
            disconnect(msg);
        break;
        default: break;
    }
}

void cleanup(){
    delete_queue(queues[0].q_id);
}

int main(void){
    if ( atexit(cleanup) < 0) 
        error("atexit()");
    if ( signal(SIGINT, handle_sigint) == SIG_ERR)        
        error("signal()");

    for (int i = 0; i < MAX_CLIENTS + 1; i++) 
        queues[i] = init_queue_id();

    if ((queues[0].key = server_key()) == -1)          
        error("server ftok");

    if ((queues[0].q_id = msgget(queues[0].key, IPC_CREAT | IPC_EXCL | 0666)) < 0)         
        error("server msgget");

    msg_t msg;
    while(counter > 0 || starting){ 
        if (msgrcv(queues[0].q_id, &msg, MSG_SIZE, -8, 0) < 0)           
            error("msgrcv");
        resolve(&msg);
    }
    return 0;
}