#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <sys/types.h>

#define MAX_ORDERS 50

void log_activity( pid_t pid, int n, char* msg, int m, int x ){
    struct timespec timestamp;
    clock_gettime(CLOCK_REALTIME, &timestamp);
    long ms = timestamp.tv_nsec/1000000;
    char time [20];
    strftime(time, 20, "%Y-%m-%d %H:%M:%S", localtime(&timestamp.tv_sec));
    printf("(%d %s.%03ld) %s: %d. Liczba zamówień do przygotowania: %d. Liczba zamówień do wysłania: %d. ", pid, time, ms, msg, n, m, x );
}


int error(char * msg){
    perror(msg);
    exit(errno);
}

int get_n();

void set_n(int n);

int get_x();

void set_x(int x);

int get_m();

void set_m(int m);