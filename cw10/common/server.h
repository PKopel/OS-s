#include "common.h"

typedef struct client{
    struct sockaddr addr;
    int socket_fd;
    int active;
    int pair_id;
    char name[20];
} client;

typedef struct pair{
    int client_o;
    int client_x;
} pair;

extern char* unix_name;
extern int unix_fd;
extern char* inet_port;
extern int inet_fd ;

extern client clients[MAX_CLIENTS];
extern int clients_number;
extern pthread_mutex_t clients_mtx;
extern int waiting_for_pair;
extern pair pairs[MAX_CLIENTS/2];
extern pthread_mutex_t pairs_mtx;


void start_server_socket(int* sock_fd, char* sock_name, int family, int protocol);

void start_server(int protocol);

void server_cleanup();

int register_client(client client, char* msg);

void remove_client(int index);

void send_ping(client client);

void send_msg(client client, char* msg);

int process_msg(client client, char* message);

void make_move(char* move);

void * socket_thread(void * arg);

void * ping_therad(void * arg);