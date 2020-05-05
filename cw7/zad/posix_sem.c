#include "common.h"

const char* sem_table_name = "table";
const char* sem_mxn_name = "mxn";
const char* shm_name = "shm";

sem_t* sem_table_desc;
sem_t* sem_mxn_desc;
int shm_desc;

void create_sem(){
    if((sem_table_desc = sem_open(sem_table_name, O_CREAT | O_EXCL, 0666, MAX_ORDERS)) == SEM_FAILED) error("create table sem");
    if((sem_mxn_desc = sem_open(sem_mxn_name, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) error("create mxn sem");
}

void get_sem(){
    if((sem_table_desc = sem_open(sem_table_name, O_RDWR)) == SEM_FAILED) error("get table sem");
    if((sem_mxn_desc = sem_open(sem_mxn_name, O_RDWR)) == SEM_FAILED) error("get mxn sem");
}

void remove_sem(){
    if(sem_unlink(sem_table_name) < 0) error("table sem_unlink");
    if(sem_unlink(sem_mxn_name) < 0) error("mxn sem_unlink");
}

void create_shm(){
    if((shm_desc = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0666 )) < 0) error("create shm_open");
    if(ftruncate(shm_desc, (MAX_ORDERS + 5)*sizeof(int)) < 0) error("ftruncate");
}

void get_shm(){
    if((shm = (int*)mmap(NULL, MAX_ORDERS + 5, PROT_READ | PROT_WRITE, MAP_SHARED, shm_desc, 0)) < 0) error("mmap");
}

void close_ipc(){
    if(munmap(shm, MAX_ORDERS + 5) < 0) error("munmap");
    if(sem_close(sem_table_desc) < 0) error("table sem_close");
    if(sem_close(sem_mxn_desc) < 0) error("mxn sem_close");
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