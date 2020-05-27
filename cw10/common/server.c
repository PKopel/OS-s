#include "server.h"

char* unix_name;
int unix_fd = -1;
char* inet_port;
int inet_fd = -1;

client clients[MAX_CLIENTS];
int clients_number;
pthread_mutex_t clients_mtx;
int waiting_for_pair;
pair pairs[MAX_CLIENTS/2];
pthread_mutex_t pairs_mtx = PTHREAD_MUTEX_INITIALIZER;

void make_move(char* move){
    char symbol;
    int client_id;
    sscanf(move, "%c %*d %d", &symbol, &client_id );
    pthread_mutex_lock(&pairs_mtx);
    pair players = pairs[clients[client_id].pair_id];
    pthread_mutex_unlock(&pairs_mtx);
    switch (symbol) {
        case 'O':
            send_msg(clients[players.client_x], move);
            break;
        case 'X':
            send_msg(clients[players.client_o], move);
            break;
        case 'E': 
            send_msg(clients[players.client_o], "end");
            send_msg(clients[players.client_x], "end");
        default:
            break;
    }
    
}

void sigint(int signum) {
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].active){
            send_msg(clients[i], "killed server");
        }
    }
    exit(0);
}

void start_server(int protocol) {
    if (atexit(server_cleanup) == -1) error("atexit");

    pthread_mutexattr_t rec_clients_mtx;

    pthread_mutexattr_init(&rec_clients_mtx);
    pthread_mutexattr_settype(&rec_clients_mtx, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&clients_mtx, &rec_clients_mtx);
    
    struct sigaction act_int;
    act_int.sa_handler = sigint;
    sigemptyset(&act_int.sa_mask);
    act_int.sa_flags = 0;
    if ((sigaction(SIGINT, &act_int, NULL)) == -1) error("sigaction");
    
    for(int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].socket_fd = -1;
        clients[i].active = 0;
        pairs[i/2].client_o = -1;
        pairs[i/2].client_x = -1;
    }
    char inet_name[15] = "localhost:";
    strcat(inet_name,inet_port);
    start_server_socket(&inet_fd, inet_name, AF_INET, protocol);
    start_server_socket(&unix_fd, unix_name, AF_LOCAL, protocol);
}

void server_cleanup() {
    if(unix_fd != -1) {
        if ((close(unix_fd)) == -1) error("unix close");
        if ((unlink(unix_name)) == -1) error("unix unlink");
    }

    if(inet_fd != -1) {
        if ((close(inet_fd)) == -1) error("inet close");
    }
}

int register_client(client client, char* msg) {
    if(clients_number >= MAX_CLIENTS)
        return -1;
    char name[20];
    sscanf(msg,"%*c %s",name);
    pthread_mutex_lock(&clients_mtx);
    int free_place;
    for(int i = 0; i< MAX_CLIENTS; i++){
        if(clients[i].socket_fd == -1) free_place = i;
        else if(strcmp(clients[i].name,name) == 0) return -2;
    }
    pthread_mutex_unlock(&clients_mtx);
    if (++clients_number % 2 == 0) {
        pthread_mutex_lock(&pairs_mtx);
        for (int i = 0; i < MAX_CLIENTS/2; i++){
            if(pairs[i].client_x == -1){
                pairs[i].client_x = waiting_for_pair;
                pairs[i].client_o = free_place;
                clients[free_place].pair_id = i;
                clients[waiting_for_pair].pair_id = i;
                send_msg(clients[waiting_for_pair], "symbol X");
                send_msg(client, "symbol O");
                break;
            }
        }
        pthread_mutex_unlock(&pairs_mtx);
    } else {
        waiting_for_pair = free_place;
        send_msg(client, "waiting");
    }
    clients[free_place].active = 1; 
    memcpy(clients[free_place].name, name, 20);
    clients[free_place].addr = client.addr;
    clients[free_place].socket_fd = client.socket_fd;
    return free_place;
}

void remove_client(int index){
    //if(close(clients[index].socket_fd) == -1) error("close socket");
    clients[index].socket_fd = -1;
    clients_number--;
    if(clients[index].pair_id != -1){
        pthread_mutex_lock(&pairs_mtx);
        pair pair = pairs[clients[index].pair_id];
        int other_id = pair.client_o == index ? pair.client_x : pair.client_o;
        printf("in pair %d %d\n",index,other_id);
        clients[other_id].pair_id = -1;
        clients[index].pair_id = -1;
        pairs[clients[index].pair_id].client_o = -1;
        pairs[clients[index].pair_id].client_x = -1;
        pthread_mutex_unlock(&pairs_mtx);
        send_msg(clients[other_id], "end");
    }
}

int process_msg(client client, char* msg) {
    int id;
    char cid[3];
    switch (msg[0])
    {
    case 'l':
        switch (id = register_client(client, msg) )
        {
        case -1:
            send_msg(client, "too many clients");
            return -1;
        case -2:
            send_msg(client, "name in use");
            return -1;
        default:  
            sprintf(cid,"%d ",id);
            printf("%s\n",cid);
            send_msg(client, cid);
            return 0;
        }
        break;
    case 'o':
        printf("remove client\n");
        sscanf(msg,"%*s %d",&id);
        remove_client(id);
        break;
    case 'X':
    case 'O':
    case 'E':
        make_move(msg);
        break;
    case 'r':
        client.active = 1;
    default:
        break;
    }
    return 0;
}


void * ping_therad(void * arg) {
    while(1) {
        sleep(10);
        pthread_mutex_lock(&clients_mtx);

        for (int i = 0; i < MAX_CLIENTS; ++i){
            if(clients[i].active == 0 && clients[i].socket_fd != -1){
                remove_client(i);
            } else if(clients[i].socket_fd != -1) {
                send_ping(clients[i]);
            }
        }

        pthread_mutex_unlock(&clients_mtx);
    }
    return (void*) 0;
}
