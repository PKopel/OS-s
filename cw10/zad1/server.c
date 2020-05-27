#include "../common/server.h"


void send_ping(client client){
    if( write(client.socket_fd, "ping", 4 ) == -1 ) error("write to socket");
    client.active = 0;
}

void send_msg(client client, char* msg){
    int msg_len = strlen(msg);
    if( write(client.socket_fd, msg, msg_len ) == -1 ) error("write to socket");
}

void start_server_socket(int* sock_fd, char* sock_name, int family, int protocol) {
    struct sockaddr sa;
    socklen_t sa_len;
    make_sockaddr(&sa, &sa_len, sock_name, family, 1);

    if ((*sock_fd = socket(family, protocol, 0)) == -1) error("socket");
    if ((bind(*sock_fd, &sa, sa_len)) == -1) error("bind");
    if ((listen(*sock_fd, SOMAXCONN)) == -1) error("listen");
}

void* socket_thread(void* arg){
    int client_fd, hwm_fd = 0;
    char buf[30];
    fd_set set, read_set;
    ssize_t nread;
    hwm_fd = unix_fd > inet_fd ? unix_fd > hwm_fd ? unix_fd : hwm_fd : hwm_fd > inet_fd ? hwm_fd : inet_fd ;
    FD_ZERO(&set);
    FD_SET(unix_fd, &set);
    FD_SET(inet_fd, &set);
    while (1) {
        read_set = set;
        if( select(hwm_fd + 1, &read_set, NULL, NULL, NULL) == -1) error("select");
        if (FD_ISSET(inet_fd, &read_set)) {
            if( (client_fd = accept(inet_fd, NULL, 0)) == -1 ) error("client accept");
            if( (nread = recv(client_fd, buf, sizeof(buf),0)) == -1 ) error("new client recv");
                if(nread > 0) {
                    client client;
                    client.socket_fd = client_fd;
                    if( process_msg(client, buf) != -1){
                        FD_SET(client_fd, &set);
                        if (client_fd > hwm_fd) hwm_fd = client_fd;
                    }
                }
        }
        if( FD_ISSET(unix_fd, &read_set)) {
            if( (client_fd = accept(unix_fd, NULL, 0)) == -1 ) error("client accept");
            if( (nread = recv(client_fd, buf, sizeof(buf),0)) == -1 ) error("new client recv");
                if(nread > 0) {
                    client client;
                    client.socket_fd = client_fd;
                    if( process_msg(client, buf) != -1){
                        FD_SET(client_fd, &set);
                        if (client_fd > hwm_fd) hwm_fd = client_fd;
                    }
                }
        }
        //pthread_mutex_lock(&clients_mtx);
        for(int i = 0; i < MAX_CLIENTS; i++){
            if(FD_ISSET(clients[i].socket_fd, &read_set)){
                if( (nread = recv(clients[i].socket_fd, buf, sizeof(buf),0)) == -1 ) error("client recv");
                if(nread == 0) {
                    FD_CLR(clients[i].socket_fd, &set);
                    if (clients[i].socket_fd == hwm_fd) hwm_fd--;
                    remove_client(i);
                } else {
                    process_msg(clients[i], buf);
                }
            }
        }
        //pthread_mutex_unlock(&clients_mtx);
    }
    return 0;
}

int main(int argc, char **argv) {

    if(argc != 3) {
        errno = 22;
        error("wrong number of arguments");
    }

    inet_port = argv[1];
    unix_name = argv[2];

    start_server(SOCK_STREAM);

    pthread_t socket_thread_id;
    pthread_create(&socket_thread_id, NULL, socket_thread, NULL);

    pthread_t ping_therad_id;
    pthread_create(&ping_therad_id, NULL, ping_therad, NULL);

    pthread_join(socket_thread_id, NULL);
    pthread_join(ping_therad_id, NULL);

    return 0;
}