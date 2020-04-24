#include "../common/common.h"

queue_id queues[MAX_CLIENTS + 1];
int starting = 1;
int counter = 0;
int current = 1;

void send(msg_t* msg, int id){
    if (mq_send((mqd_t)queues[id].q_id, msg->message, MSG_MAX, 10 - msg->type) == -1) 
        error("server send");
}

void delete_queue(int q_id){
    if (mq_close(q_id) < 0) 
        error("closing queue");
    if (mq_unlink(server_name) < 0)
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
        if ((int)mq_receive((mqd_t)queues[0].q_id, msg.message, MSG_MAX, (unsigned int *)&(msg.type)) < 0)           
            error("server receive");
        if(10 - msg.type == STOP ) counter--;
    }
    exit(EXIT_SUCCESS);
}

void init(msg_t* msg){
    int client_queue_id;
    if ((client_queue_id = (int)mq_open(msg->message, O_WRONLY)) == -1) 
            error("client queue");
    msg->type = RESPONSE;
    if (counter < MAX_CLIENTS){
        printf("new client #%d\n", current);
        msg->message[0] = current;
        queues[current].q_id = client_queue_id;
        counter++;
        starting = 0;
    } else {
        msg->message[0] = -1;
    }
    send(msg,current);
    while (queues[current].q_id > 0 && current < MAX_CLIENTS) current++;
}

void stop(msg_t* msg){
    int i = msg->message[0];
    if(msg->message[1]){
        int connected_id = msg->message[2];
        queues[connected_id].aviable = 1;
        msg->type = DISCONNECT;
        msg->message[0] = 0;
        send(msg,connected_id);
    }
    counter--;
    current = i;
    queues[i].q_id = -1;
}

void list(msg_t* msg){
    int j = 2;
    msg->message[1] = counter;
    for (int i = 1; i < MAX_CLIENTS; ++i){
        if (queues[i].q_id > 0 && i != msg->message[0])
        msg->message[j++] = i;
        msg->message[j++] = queues[i].aviable;
    }
    msg->type = RESPONSE;
    msg->message[0] = 0;
    send(msg,msg->message[0]);
}

void connect(msg_t * msg){
    int receiver_id = msg->message[1];
    int sender_id = msg->message[0];
    if (queues[receiver_id].aviable && queues[sender_id].aviable){
        queues[sender_id].aviable = 0;
        queues[receiver_id].aviable = 0;
        send(msg,receiver_id);
    } else {
        msg->type = RESPONSE;
        msg->message[0] = -1;
        send(msg,sender_id);
    }
}

void disconnect(msg_t* msg){
    int receiver_id = msg->message[1];
    int sender_id = msg->message[0];
    queues[sender_id].aviable = 1;
    queues[receiver_id].aviable = 1;
}

void resolve(msg_t * msg){
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

    struct mq_attr attr;
    attr.mq_maxmsg = MAX_CLIENTS;
    attr.mq_msgsize = MSG_MAX;

    if ((queues[0].q_id = (int)mq_open(server_name, O_RDONLY | O_CREAT |O_EXCL, 0666, &attr)) < 0)         
        error("server mq_open");

    msg_t msg;
    while(counter > 0 || starting){ 
        if ((int)mq_receive((mqd_t)queues[0].q_id, msg.message, MSG_MAX, (unsigned int *)&(msg.type)) < 0)           
            error("mq_receive");
        msg.type = 10 - (unsigned int)msg.type;
        resolve(&msg);
    }
    return 0;
}