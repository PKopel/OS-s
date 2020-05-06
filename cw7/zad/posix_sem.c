#include "common.h"

const char* sem_table_name = "table";
const char* sem_m_name = "mxn_m";
const char* sem_x_name = "mxn_x";
const char* sem_n_name = "mxn_n";
const char* shm_name = "shm";

sem_t* sem_table_desc;
sem_t* sem_m_desc;
sem_t* sem_x_desc;
sem_t* sem_n_desc;
int shm_desc;

void create_sem(){
    if((sem_table_desc = sem_open(sem_table_name, O_CREAT | O_EXCL, 0666, MAX_ORDERS)) == SEM_FAILED) error("create table sem");
    if((sem_n_desc = sem_open(sem_n_name, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) error("create n sem");
    if((sem_x_desc = sem_open(sem_x_name, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) error("create x sem");
    if((sem_m_desc = sem_open(sem_m_name, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) error("create m sem");
}

void get_sem(){
    if((sem_table_desc = sem_open(sem_table_name, 0)) == SEM_FAILED) error("get table sem");
    if((sem_n_desc = sem_open(sem_n_name, 0)) == SEM_FAILED) error("get n sem");
    if((sem_x_desc = sem_open(sem_x_name, 0)) == SEM_FAILED) error("get x sem");
    if((sem_m_desc = sem_open(sem_m_name, 0)) == SEM_FAILED) error("get m sem");
}

void remove_sem(){
    if(sem_unlink(sem_table_name) < 0) error("table sem_unlink");
    if(sem_unlink(sem_m_name) < 0) error("m sem_unlink");
    if(sem_unlink(sem_x_name) < 0) error("x sem_unlink");
    if(sem_unlink(sem_n_name) < 0) error("n sem_unlink");
}

void create_shm(){
    if((shm_desc = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0666 )) < 0) error("create shm_open");
    if(ftruncate(shm_desc, (MAX_ORDERS + 5)*sizeof(int)) < 0) error("ftruncate");
    get_shm();
    for (int i = 0; i < 5; i++) shm[i] = 0;
    for (int i = 5; i < MAX_ORDERS + 5; i++) shm[i] = -1;
}

void get_shm(){
    if((shm_desc = shm_open(shm_name, O_RDWR, 0666 )) < 0) error("get shm_open");
    if((shm = (int*)mmap(0, (MAX_ORDERS + 5)*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_desc, 0)) < 0) error("mmap");
}

void close_ipc(){
    if(munmap(shm, MAX_ORDERS + 5) < 0) error("munmap");
    if(sem_close(sem_table_desc) < 0) error("table sem_close");
    if(sem_close(sem_m_desc) < 0) error("m sem_close");
    if(sem_close(sem_x_desc) < 0) error("x sem_close");
    if(sem_close(sem_n_desc) < 0) error("n sem_close");
}

void remove_shm(){
    if(shm_unlink(shm_name) < 0) error("shm_unlink");
}

void mxn_get_sem(action action_m, action action_x, action action_n){
    if(action_n == SET){
        if(sem_wait(sem_table_desc) < 0) error("table sem_wait");
    }
    if(action_m == DEC){
        if(sem_wait(sem_m_desc) < 0) error("table sem_wait");
    }
    if(action_x == DEC){
        if(sem_wait(sem_x_desc) < 0) error("table sem_wait");
    }
    if(sem_wait(sem_n_desc) < 0) error("n sem_wait");
}

void mxn_return_sem(action action_m, action action_x, action action_n){
    if(sem_post(sem_n_desc) < 0) error("n sem_post");
    if(action_x == DEC){
        if(sem_post(sem_table_desc) < 0) error("table sem_post");
    }
    if(action_m == INC){
        if(sem_post(sem_m_desc) < 0) error("table sem_wait");
    }
    if(action_x == INC){
        if(sem_post(sem_x_desc) < 0) error("table sem_wait");
    }
}