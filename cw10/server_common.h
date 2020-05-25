#include "common.h"

typedef struct client{
    struct sockaddr addr;
    int socket_fd;
    int active;
    int pair_id;
    char* name;
} client;

typedef struct pair{
    int client_o;
    int client_x;
} pair;

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


void start_server_socket(int* sock_fd, char* sock_name, int family, int protocol);

void start_server(int protocol);

void server_cleanup();

int register_client(client client, char* msg);

int send_ping(client client);

int send_msg(client client, char* msg);

int process_msg(client client, char* message);

void make_move(char* move);

void * socket_thread(void * arg);

void * ping_therad(void * arg);