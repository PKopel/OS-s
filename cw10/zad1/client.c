#include "../client_common.h"

void send_msg(char* msg){
    if( write(server_fd, msg, strlen(msg) + 1 ) == -1) error("write");
}

int main(int argc, char** argv){
    if(argc != 4){
        errno = 22;
        error("wrong number of arguments");
    }

    name = argv[1];
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