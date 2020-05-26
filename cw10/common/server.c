#include "server.h"

char* unix_name;
int unix_fd = -1;
char* inet_port;
int inet_fd = -1;

client clients[MAX_CLIENTS];
int clients_number;
pthread_mutex_t clients_mtx = PTHREAD_MUTEX_INITIALIZER;
int waiting_for_pair;
pair pairs[MAX_CLIENTS/2];
pthread_mutex_t pairs_mtx = PTHREAD_MUTEX_INITIALIZER;

void make_move(char* move){
    char symbol;
    int pair_id, field;
    sscanf(move, "%c %d %d", &symbol, &pair_id, &field );
    pthread_mutex_lock(&pairs_mtx);
    pair players = pairs[pair_id];
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
    printf("SIGINT received - closing server\n");
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].active){
            send_msg(clients[i], "killed server");
        }
    }
    exit(0);
}

void start_server(int protocol) {
    if (atexit(server_cleanup) == -1) error("atexit");
    
    struct sigaction act_int;
    act_int.sa_handler = sigint;
    sigemptyset(&act_int.sa_mask);
    act_int.sa_flags = 0;
    if ((sigaction(SIGINT, &act_int, NULL)) == -1) error("sigaction");
    
    for(int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].socket_fd = -1;
        clients[i].name = NULL;
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
    client.name = name;
    client.active = 1;
    client.pair_id = -1;
    pthread_mutex_lock(&clients_mtx);
    int free_place;
    for(int i = 0; i< MAX_CLIENTS; i++){
        if(clients[i].socket_fd == -1) free_place = i;
        else if(strcmp(clients[i].name,client.name) == 0) return -2;
    }
    clients[free_place] = client;
    pthread_mutex_unlock(&clients_mtx);
    if (++clients_number %2 == 0) {
        pthread_mutex_lock(&pairs_mtx);
        for (int i = 0; i < MAX_CLIENTS/2; i++){
            if(pairs[i].client_x == -1){
                pairs[i].client_x = waiting_for_pair;
                pairs[i].client_o = free_place;
                send_msg(clients[waiting_for_pair], "symbol X");
                send_msg(clients[free_place], "symbol O");
                break;
            }
        }
        pthread_mutex_unlock(&pairs_mtx);
    } else {
        waiting_for_pair = free_place;
        send_msg(client, "waiting");
    }
    return free_place;
}

void remove_client(int index){
    client closing = clients[index];
    if(close(closing.socket_fd) == -1) error("close socket");
    closing.socket_fd = -1;
    clients_number--;
    if(closing.pair_id != -1){
        pthread_mutex_lock(&pairs_mtx);
        pair pair = pairs[closing.pair_id];
        int other_id = pair.client_o == index ? pair.client_x : pair.client_o;
        client other = clients[other_id];
        other.pair_id = -1;
        closing.pair_id = -1;
        pair.client_o = -1;
        pair.client_x = -1;
        pthread_mutex_unlock(&pairs_mtx);
        send_msg(other, "end");
    }
}

int process_msg(client client, char* msg) {
    char id;
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
            send_msg(client, &id);
            return 0;
        }
        break;
    case 'o':
        remove_client(msg[1]);
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
            if(clients[i].active == 0){
                remove_client(i);
            } else if(clients[i].socket_fd != -1) {
                send_ping(clients[i]);
            }
        }

        pthread_mutex_unlock(&clients_mtx);
    }
    return (void*) 0;
}
