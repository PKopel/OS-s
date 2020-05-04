#include "common.h"

const char* sem_table_name = "/tmp/sem/table";
const char* sem_mxn_name = "/tmp/sem/mxn";
const char* shm_name = "/tmp/shm";

sem_t* sem_table_desc;
sem_t* sem_mxn_desc;
int shm_desc;

void creat_sem(){
    if((sem_table_desc = sem_open(sem_table_name, O_CREAT | O_EXCL, S_IRWXO, MAX_ORDERS)) == SEM_FAILED) error("creat sem_open");
    if((sem_mxn_desc = sem_open(sem_mxn_name, O_CREAT | O_EXCL, S_IRWXO, 1)) == SEM_FAILED) error("creat sem_open");
}

void get_sem(){
    if((sem_table_desc = sem_open(sem_table_name, O_RDWR)) == SEM_FAILED) error("creat sem_open");
    if((sem_mxn_desc = sem_open(sem_mxn_name, O_RDWR)) == SEM_FAILED) error("creat sem_open");
}

void remove_sem(){
    if(sem_close(sem_table_desc) < 0) error("table sem_close");
    if(sem_unlink(sem_table_name) < 0) error("table sem_unlink");
    if(sem_close(sem_mxn_desc) < 0) error("mxn sem_close");
    if(sem_unlink(sem_mxn_name) < 0) error("mxn sem_unlink");
}

void creat_shm(){
    if((shm_desc = shm_open(shm_name, O_CREAT | O_EXCL, S_IRWXO )) < 0) error("creat shm_open");
    if(ftruncate(shm_desc, MAX_ORDERS + 5) < 0) error("ftruncate");
}

void get_shm(){
    if((shm = (int*)mmap(NULL, MAX_ORDERS + 5, PROT_READ | PROT_WRITE, MAP_SHARED, shm_desc, 0)) < 0) error("mmap");
}

void close_shm(){
    if(munmap(shm, MAX_ORDERS + 5) < 0) error("munmap");
}

void remove_shm(){
    if(shm_unlink(shm_name) < 0) error("shm_unlink");
}

void mxn_get_sem(action action_n){
    if(sem_wait(sem_mxn_desc) < 0) error("mxn sem_wait");
    if(action_n == SET){
        if(sem_trywait(sem_table_desc) < 0) error("table sem_wait");
    }
}

void mxn_return_sem(action action_x){
    if(sem_post(sem_mxn_desc) < 0) error("mxn sem_post");
    if(action_x == DEC){
        if(sem_post(sem_table_desc) < 0) error("table sem_post");
    }
}