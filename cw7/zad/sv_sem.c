#include "common.h"

typedef union semun {
    int val;
    struct semid_ds* buf;
    unsigned short* array;
} semun;

int sem_desc;
int* shm;

void get_sem(){
    key_t sem_key = ftok("/tmp",0x111);
    sem_desc = semget(sem_key, 2, IPC_CREAT | 0666);
    union semun sem_un;
    sem_un.val = MAX_ORDERS;
    semctl(sem_desc, 0,SETVAL, sem_un);
    sem_un.val = 1;
    semctl(sem_desc, 1,SETVAL, sem_un);
}

void get_shm(){
    key_t shm_key = ftok("/tmp",0x123);
}

void close_sem(int desc){

}

void close_shm(void* shm, int desc){
    
}

void mxn_get_sem(action action_n){

}

void mxn_return_sem(action action_x){

}

int get_n(){

}

void set_n(int n){

}

int get_m(){

}

void set_m(int m){

}

int get_x(){

}

void set_x(int x){

}