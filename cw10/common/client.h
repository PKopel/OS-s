#include "common.h"

int client_fd;
char* server_name;
char* client_name;
int pair_id;

char board[9];
char symbol;

int family;

void send_msg(char* msg);

void process_msg(char* msg);

void start_client(int family, int protocol);

void start_client_socket(int* sock_fd, int family, int protocol);


