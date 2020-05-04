#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
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

void mxn_get_sem(action action_n);

void mxn_return_sem(action action_x);

int get_n();

void set_n(int n);

int get_m();

void set_m(int m);

int get_x();

void set_x(int x);

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