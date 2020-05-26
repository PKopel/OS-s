#include "../common/client.h"

void start_client_socket( int* sock_fd, int family, int protocol){
    struct sockaddr sa;
    socklen_t sa_len;
    make_sockaddr(&sa, &sa_len, server_name, family, 0);

    if( sock_fd = socket(family, protocol, 0) == -1 ) error("socket");
    while (connect(sock_fd, &sa, &sa_len) == -1)
        if (errno == ENOENT) sleep(1);
        else error("connect");
}

void send_msg(char* msg){
    if( write(server_fd, msg, strlen(msg) + 1 ) == -1) error("write");
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