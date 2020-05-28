#include "../common/server.h"

void send_ping(client client){
    if( sendto(client.socket_fd, "ping", 4, 0, &client.addr, sizeof(client.addr) ) == -1 ) error("write to socket");
    client.active = 0;
}

void send_msg(client client, char* msg){
    int msg_len = strlen(msg);
    if( sendto(client.socket_fd, msg, msg_len, 0, &client.addr, sizeof(client.addr) ) == -1 ) error("write to socket");
}

void start_server_socket(int* sock_fd, char* sock_name, int family, int protocol) {
    struct sockaddr sa;
    socklen_t sa_len;
    make_sockaddr(&sa, &sa_len, sock_name, family, 1);

    if ((*sock_fd = socket(family, protocol, 0)) == -1) error("socket");
    if ((bind(*sock_fd, &sa, sa_len)) == -1) error("bind");
    if ( fcntl(*sock_fd, F_SETFL, O_NONBLOCK, 1 ) == -1 ) error("fcntl");
}

int find_client(struct sockaddr addr){
    pthread_mutex_lock(&clients_mtx);
    for(int i = 0; i < MAX_CLIENTS; i++){
        if ( clients[i].socket_fd != -1
        && addr.sa_data == clients[i].addr.sa_data 
        && addr.sa_family == clients[i].addr.sa_family)
            return i;
    }
    pthread_mutex_unlock(&clients_mtx);
    return -1;
}

void* socket_thread(void* arg){
    struct sockaddr sa;
    socklen_t sa_len = sizeof(sa);
    int client_id;
    char buf[30];
    ssize_t nrecv;
    client client;
    while (1) {
        if( (nrecv = recvfrom(unix_fd, buf, sizeof(buf), O_NONBLOCK, &sa, &sa_len)) == -1 && errno != EAGAIN && errno != EWOULDBLOCK) error("recvfrom");
        if(nrecv > 0){
            if(( client_id = find_client(sa)) == -1) {
                client.socket_fd = unix_fd;
                client.addr = sa;
            } else client = clients[client_id];
            process_msg(client, buf);
        }
        if( (nrecv = recvfrom(inet_fd, buf, sizeof(buf),  O_NONBLOCK, &sa, &sa_len)) == -1 && errno != EAGAIN && errno != EWOULDBLOCK) error("recvfrom");
        if(nrecv > 0){
            if(( client_id = find_client(sa)) == -1) {
                client.socket_fd = inet_fd;
                client.addr = sa;
            } else client = clients[client_id];
            process_msg(client, buf);
        }
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

    start_server(SOCK_DGRAM);

    pthread_t socket_thread_id;
    pthread_create(&socket_thread_id, NULL, socket_thread, NULL);

    pthread_t ping_therad_id;
    pthread_create(&ping_therad_id, NULL, ping_therad, NULL);

    pthread_join(socket_thread_id, NULL);
    pthread_join(ping_therad_id, NULL);

    return 0;
}