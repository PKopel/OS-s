#include "common.h"

void log_activity( pid_t pid, int n, char* msg, int m, int x ){
    struct timespec timestamp;
    clock_gettime(CLOCK_REALTIME, &timestamp);
    long ms = timestamp.tv_nsec/1000000;
    char time [20];
    strftime(time, 20, "%Y-%m-%d %H:%M:%S", localtime(&timestamp.tv_sec));
    printf("(%d %s.%03ld) %s: %d. Liczba zamówień do przygotowania: %d. Liczba zamówień do wysłania: %d.\n", pid, time, ms, msg, n, m, x );
}


int error(char * msg){
    perror(msg);
    exit(errno);
}

void init_worker(int worker_stage){
    n_stage = worker_stage;
    get_sem();
    get_shm();
    if(atexit(close_ipc) < 0) error("atexit");
    printf("worker initialized\n");
}

int get_n(){
    int first_n = shm[n_stage];
    shm[n_stage] = (shm[n_stage] + 1)%MAX_ORDERS;
    return shm[first_n + 5];
}

void set_n(int n){
    int first_empty = shm[0];
    shm[0] = (shm[0] + 1)%MAX_ORDERS;
    shm[first_empty + 5] = n;
}

int get_m(){
    return shm[3];
}

void set_m(int m){
    shm[3] = m;
}

int get_x(){
    return shm[4];
}

void set_x(int x){
    shm[4] = x;
}

void interact(int* num, int (*getter)(void), void (*setter)(int)){
    switch (num[1])
    {
    case GET: num[0] = getter();break;
    case SET: setter(num[0]); break;
    case DEC: setter(num[0] = getter() - 1); break;
    case INC: setter(num[0] = getter() + 1); break;
    default: break;
    }
}

void mxn_interact(int mxn[3][2]){
    mxn_get_sem(mxn[2][1]);
    interact(mxn[0],get_m,set_m);
    interact(mxn[1],get_x,set_x);
    interact(mxn[2],get_n,set_n);
    mxn_return_sem(mxn[1][1]);
}