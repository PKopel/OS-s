#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

#define MAX_ORDERS 50

typedef enum action{
    GET = 1,
    SET = 2,
    INC = 3,
    DEC = 4
} action;

/*
    shm[0] - first empty slot in order table
    shm[1] - first unpacked order
    shm[2] - first not send order
    shm[3] - m value
    shm[4] - x value
    shm[5+] - order table
*/
int* shm;
int n_stage;

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

void creat_sem();

void creat_shm();

void get_sem();

void get_shm();

void remove_sem();

void remove_shm();

void close_shm();

void init_worker(int worker_stage){
    n_stage = worker_stage;
    get_sem();
    get_shm();
    if(atexit(close_shm) < 0) error("atexit");
}

void mxn_get_sem(action action_n);

void mxn_return_sem(action action_x);

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

void mxn_interact(int** mxn){
    mnx_get_sem(mxn[2][1]);
    interact(mxn[0],get_m,set_m);
    interact(mxn[1],get_x,set_x);
    interact(mxn[2],get_n,set_n);
    mxn_return_sem(mxn[1][1]);
}