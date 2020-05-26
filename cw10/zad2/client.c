#include "../client_common.h"

struct sockaddr server_addr;
struct sockaddr client_addr;

void start_client_socket( int* sock_fd, int family, int protocol){
    socklen_t server_len;
    make_sockaddr(&server_addr, &server_len, server_name, family, 0);

    socklen_t client_len;
    make_sockaddr(&client_addr, &client_len, client_name, family, 0);

    if( sock_fd = socket(family, protocol, 0) == -1 ) error("socket");
    if( bind(sock_fd, &client_addr, client_len) == -1) error("bind");
}

void send_msg(char* msg){
    if( sendto(server_fd, msg, strlen(msg) + 1, 0, &server_addr, sizeof(server_addr) ) == -1) error("write");
}

int main(int argc, char** argv){
    if(argc != 4){
        errno = 22;
        error("wrong number of arguments");
    }

    client_name = argv[1];
    int family;
    if(strcmp(argv[2],"unix") == 0) family = AF_LOCAL;
    if(strcmp(argv[2],"inet") == 0) family = AF_INET;
    server_name = argv[3];

    start_client(family, SOCK_STREAM);
    char buf[30];
    while(1){
      if( read(server_fd, buf, sizeof(buf)) == -1 ) error("read");  
      process_msg(buf);
    }
}