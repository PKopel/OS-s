#include "../common/common.h"

queue_id server;
queue_id client;
queue_id connected;

int client_id = -1;
char client_name[40];

void send(msg_t* msg, int q_id){
    if (mq_send(q_id, msg->message, MSG_MAX, 10 - msg->type) == -1) 
        error("server send");
}

void stop(){
    msg_t msg;
    msg.message[0] = client_id;
    msg.type = STOP;
    if(connected.q_id > 0){
        msg.message[1] = 1;
        msg.message[2] = connected.aviable;
    } else msg.message[1] = 0;
    send(&msg,server.q_id);
    exit(0);
}

void list(){
    msg_t msg;
    msg.message[0] = client_id;
    msg.type = LIST;
    send(&msg,server.q_id);
    if ( mq_receive(client.q_id, msg.message, MSG_MAX, (unsigned int *)&(msg.type)) < 0)     
        error("client receive");
    msg.type = 10 - (unsigned int)msg.type;
    printf("received type %ld\n",msg.type);
    if(msg.type == RESPONSE){
        int clients_number = msg.message[1];
        for (int i = 2; i < clients_number*2; i+=2){
            if (msg.message[i] != client_id)
            printf("%d, aviable: %d\n",(int)msg.message[i], (int)msg.message[i+1]);
        }
    } else if(msg.type == FINISH){
        stop();
    }
}

void connect(int connect_id){
    msg_t msg;
    connected.aviable = connect_id;
    msg.message[0] = client_id;
    msg.type = CONNECT;
    msg.message[1] = connect_id;
    strcat(msg.message,client_name);
    send(&msg,server.q_id);
    if ( mq_receive(client.q_id, msg.message, MSG_MAX, (unsigned int *)&(msg.type)) < 0)     
        error("client receive");
    msg.type = 10 - (unsigned int)msg.type;
    if(msg.type == CONNECT){
        if( (int)msg.message[0] != -1){
            if ((connected.q_id = mq_open(msg.message, O_WRONLY)) < 0)                 
                error("connected queue");
            else 
                printf("connected to %d\n", connect_id);
        }
    } else if(msg.type == RESPONSE){
        printf("can't connect\n");
    } else if(msg.type == FINISH){
        stop();
    }
}

void connect_(msg_t msg){
    connected.key = msg.message[0];
    printf("connection attempt from %d\n", (int)msg.message[1]);
    char* connect_name = msg.message + 2;
    if( connected.key != -1){
        if ((connected.q_id = mq_open(connect_name, O_WRONLY)) < 0)                 
                error("connected queue");
        printf("connected to %d\n", (int)msg.message[1]);
        sprintf(msg.message,"%d%s",client_id,client_name);
        send(&msg, connected.q_id);
        connected.aviable = (int)msg.message[1];
    }
}

void disconnect(){
    msg_t msg;
    msg.message[0] = client_id;
    msg.message[1] = connected.aviable;
    msg.type = DISCONNECT;
    send(&msg, server.q_id);
    send(&msg, connected.q_id);
    connected.q_id = -1;
}

void disconnect_(msg_t msg){
    connected.q_id = -1;
}

void send_msg(char* text){
    if(connected.q_id != -1){
        msg_t msg;
        msg.message[0] = client_id;
        msg.type = RESPONSE;
        strcpy(msg.message, text);
        send(&msg, connected.q_id);
    }
}

void check_response(msg_t msg){
    printf("from %d\n", msg.message[0]);
    printf("%s\n", msg.message);
}

void handle_sigint(int signum){
    stop();
}

void check_msg(){
    msg_t msg;
    printf("checking messages\n");
    struct timespec timeout;
    timeout.tv_sec = 1;
    timeout.tv_nsec = 1;
    int result = mq_timedreceive(client.q_id, msg.message, MSG_MAX, (unsigned int *)&(msg.type),&timeout);
    msg.type = 10 - (unsigned int)msg.type;
    if(result < 0 && errno == ETIMEDOUT) return;
    else if(result < 0) error("client receive");
    switch(msg.type){
        case CONNECT:
            connect_(msg);
        break;
        case DISCONNECT:
            disconnect_(msg);
        break;
        case FINISH:
            stop();
        break;
        case RESPONSE:
            check_response(msg);
        default: break;
    }
}


void resolve(char * cmd, int length){
    char* first, *rest;
    first = strtok_r(cmd, " \n",&rest);
    if (strcmp(first, "LIST") == 0) list();
    else if (strcmp(first, "CONNECT") == 0) connect(atoi(strtok_r(NULL," ",&rest)));
    else if (strcmp(first, "DISCONNECT") == 0) disconnect();
    else if (strcmp(first, "STOP") == 0) stop();
    else if (strcmp(first, "SEND")) send_msg(cmd);
    else if (strcmp(first, "CHECK")) check_msg();
    else printf("unknown\n");
}

void init_client(){
    msg_t msg;
    msg.type = INIT;
    msg.message[0] = client.key;
    strcpy(msg.message, client_name);
    if ( mq_send(server.q_id, msg.message, MSG_MAX, 10 - INIT) < 0)      
        error("client init");
    if ( mq_receive(client.q_id, msg.message, MSG_MAX, NULL) < 0)     
        error("client response");
    if(msg.message[0] > 0 ){
        client_id = msg.message[0];
    } else error("server full");
}

void delete_queue(int q_id){
    if (mq_close(q_id) < 0) 
        error("closing queue");
    if (mq_unlink(client_name) < 0)
        error("deleting queue");
}

void cleanup(){
    delete_queue(client.q_id);
}

int main(void){
    if ( atexit(cleanup) < 0)                  
        error("atexit");
    if ( signal(SIGINT, handle_sigint) == SIG_ERR)  
        error("signal");

    server = init_queue_id();
    client = init_queue_id();
    connected = init_queue_id();

    struct mq_attr attr;
    attr.mq_maxmsg = MAX_CLIENTS;
    attr.mq_msgsize = MSG_MAX;

    sprintf(client_name,"/cli_cw6_%d",getpid());
    if ((client.q_id = mq_open(client_name, O_RDONLY | O_CREAT |O_EXCL, 0666, &attr)) < 0)                 
        error("client mq_open");

    if ((server.q_id = mq_open(server_name, O_WRONLY)) < 0)                 
        error("server mq_open");

    init_client();

    printf("client initialized #%d\n", client_id);
    char * buff;
    size_t len = 0;
    while(1){
        check_msg(server.q_id);
        printf("%d>\n", client_id);
        getline(&buff, &len, stdin);
        resolve(buff,len);
    }
    free((void*)buff);

    return 0;
}