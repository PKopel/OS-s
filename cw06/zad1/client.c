#include "../common/common.h"

queue_id server;
queue_id client;
queue_id connected;

int client_id = -1;


void delete_queue(int q_id){
    if (msgctl(q_id, IPC_RMID, NULL) < 0) 
        error("deleting queue");
}

void send(msg_t* msg, int q_id){
     if (msgsnd(q_id, msg, MSG_SIZE, 0) == -1) 
        error("client send");
}

void stop(){
    msg_t msg;
    msg.sender_id = client_id;
    msg.type = STOP;
    if(connected.q_id > 0){
        msg.message[0] = 1;
        msg.message[1] = connected.aviable;
    } else msg.message[0] = 0;
    send(&msg,server.q_id);
    exit(0);
}

void list(){
    msg_t msg;
    msg.sender_id = client_id;
    msg.type = LIST;
    send(&msg,server.q_id);
    if ( msgrcv(client.q_id, &msg, MSG_SIZE, -8, 0) < 0)     
        error("client receive");
    printf("received type %ld\n",msg.type);
    if(msg.type == RESPONSE){
        int clients_number = msg.message[0];
        for (int i = 0; i < clients_number*2; i+=2){
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
    msg.sender_id = client_id;
    msg.type = CONNECT;
    msg.message[0] = connect_id;
    send(&msg,server.q_id);
    if ( msgrcv(client.q_id, &msg, MSG_SIZE, -8, 0) < 0)     
        error("client receive");
    if(msg.type == CONNECT){
        connected.key = msg.sender_id;
        if( connected.key != -1){
            if ((connected.q_id = msgget(connected.key, 0)) < 0)                 
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
    connected.key = msg.sender_id;
    printf("connection attempt from %d\n", (int)msg.message[0]);
    if( connected.key != -1){
        if ((connected.q_id = msgget(connected.key, 0)) < 0)                 
            error("connected queue");
        printf("connected to %d\n", (int)msg.message[0]);
        connected.aviable = (int)msg.message[0];
    }
}

void disconnect(){
    msg_t msg;
    msg.sender_id = client_id;
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
        msg.sender_id = client_id;
        msg.type = RESPONSE;
        strcpy(msg.message, text);
        send(&msg, connected.q_id);
    }
}

void check_response(msg_t msg){
    printf("from %d\n", msg.sender_id);
    printf("%s\n", msg.message);
}

void handle_sigint(int signum){
    stop();
}

void check_msg(){
    msg_t msg;
    printf("checking messages\n");
    int result = msgrcv(client.q_id, &msg, MSG_SIZE, -8, IPC_NOWAIT);
    if(result < 0 && errno == ENOMSG) return;
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
    msg.sender_id = client.key;

    if ( msgsnd(server.q_id, &msg, MSG_SIZE, 0) < 0)      
        error("client init");
    if ( msgrcv(client.q_id, &msg, MSG_SIZE, RESPONSE, 0) < 0)     
        error("client response");
    if(msg.sender_id > 0 ){
        client_id = msg.sender_id;
    } else error("server full");
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

    if ((client.key = ftok("/tmp", getpid())) == -1)              
        error("client ftok");

    if ((client.q_id = msgget(client.key, IPC_CREAT | IPC_EXCL | 0666)) < 0)                 
        error("client msgget");

    if ((server.key = server_key()) == -1)              
        error("server ftok");

    if ((server.q_id = msgget(server.key, 0)) < 0)                 
        error("server msgget");

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