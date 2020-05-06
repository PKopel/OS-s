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

#define MAX_ORDERS 40

typedef enum action{
    GET = 1,
    SET = 2,
    INC = 4,
    DEC = 6
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

void log_activity( pid_t pid, int n, char* msg, int m, int x );

void log_debug(char* msg);

int error(char * msg);

void create_sem();

void create_shm();

void get_sem();

void get_shm();

void remove_sem();

void remove_shm();

void close_ipc();

void init_worker(int worker_stage);

void mxn_get_sem(action action_m, action action_x, action action_n);

void mxn_return_sem(action action_x);

int get_n();

void set_n(int n);

int get_m();

void set_m(int m);

int get_x();

void set_x(int x);

void interact(int* num, int (*getter)(void), void (*setter)(int));

void mxn_interact(int mxn[3][2]);