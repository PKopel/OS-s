#include "client_common.h"

void sigint(int signum){
    send_msg("over");
    exit(0);
}

void start_client(int family, int protocol){
    struct sigaction act_int;
    act_int.sa_handler = sigint;
    sigemptyset(&act_int.sa_mask);
    act_int.sa_flags = 0;
    if ((sigaction(SIGINT, &act_int, NULL)) == -1) error("sigaction");
    
    start_client_socket( &server_fd, family, protocol);

    char login_msg[30];
    sprintf(login_msg,"l %s",client_name);
    send_msg(login_msg);
}
    

void client_cleanup() {
    if(family == AF_LOCAL) {
        if ((close(server_fd)) == -1) error("unix close");
        if ((unlink(server_name)) == -1) error("unix unlink");
    }

    if(family == AF_INET) {
        if ((close(server_fd)) == -1) error("inet close");
    }
}

void print_board(){
    printf("%c | %c | %c\n---------\n", board[0], board[1], board[2]);
    printf("%c | %c | %c\n---------\n", board[3], board[4], board[5]);
    printf("%c | %c | %c\n", board[6], board[7], board[8]);
}

int check_end(){
    int end = 0;
    char winner;
    for(int i = 0; i < 3; i++){
        if(board[i] == board[i+1] && board[i] == board[i+2] && board[0] != '-'){
            winner = board[i];
            end+=1;
        }
        if(board[i] == board[i+3] && board[i] == board[i+6] && board[0] != '-'){
            winner = board[i];
            end+=1;
        }
    }
    if(board[0] == board[4] && board[0] == board[8] && board[0] != '-'){
            winner = board[0];
            end+=1;
        }
    if(board[2] == board[4] && board[2] == board[6] && board[0] != '-'){
            winner = board[2];
            end+=1;
        }
    printf("Won player %c\n", winner);
    return end;
}

void read_move(){
    int field;
    do {
        printf("enter field number (1-9 from upper left):\n");
        scanf("%d",&field);
    } while (board[field-1] != '-');
    char move[7];
    sprintf(move,"%c %d %d", symbol, pair_id, field-1);
    send_msg(move);
}

void make_move(char* msg){
    int field;
    char new_symbol;
    sscanf(msg,"%c %*d %d",&new_symbol,&field);
    board[field] = new_symbol;
    print_board();
    if(check_end()) send_msg("E");
    else read_move();
}

void process_msg(char* msg){
     switch (msg[0])
    {
    case 'X':
    case 'O':
        make_move(msg);
        break;
    case 'e':
        send_msg("over");
        exit(0);
        break;
    case 'p':
        send_msg("response");
        break;
    case 'w':
        printf("waiting for partner\n");
        break;
    case 's': 
        sscanf(msg, "%*s, %c", &symbol);
        printf("starting game, your symbol: %c\n", symbol);
        break;
    case 'k':
    case 't':
    case 'n':
        printf("%s\n", msg);
        exit(0);
    default:
        break;
    }
}