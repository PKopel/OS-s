#include "common.h"

int server_fd;
char* server_name;
char* name;
int pair_id;

char board[9];
char symbol;

int family;

void send_msg(char* msg);

void process_msg(char* msg);

void start_client(int family, int protocol);


