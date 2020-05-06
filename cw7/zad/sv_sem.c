#include "common.h"

typedef union semun {
    int val;
    struct semid_ds* buf;
    unsigned short* array;
} semun;

int sem_desc;
int shm_desc;


void create_sem(){
    key_t sem_key = ftok("/tmp",0x888);
    if((sem_desc = semget(sem_key, 4, IPC_CREAT | IPC_EXCL | 0666)) < 0) error("create semget");
    union semun sem_un;
    sem_un.val = MAX_ORDERS;
    if(semctl(sem_desc, 0, SETVAL, sem_un) < 0) error("create semctl");
    sem_un.val = 1;
    if(semctl(sem_desc, 1, SETVAL, sem_un) < 0) error("create semctl");
    sem_un.val = 0;
    if(semctl(sem_desc, 2, SETVAL, sem_un) < 0) error("create semctl");
    if(semctl(sem_desc, 3, SETVAL, sem_un) < 0) error("create semctl");
}

void get_sem(){
    key_t sem_key = ftok("/tmp",0x888);
    if((sem_desc = semget(sem_key, 0, 0)) < 0) error("get semget");
}

void remove_sem(){
    if(semctl(sem_desc, 0, IPC_RMID, NULL) < 0) error("remove semctl");
}

void create_shm(){
    key_t shm_key = ftok("/tmp",0x999);
    if((shm_desc = shmget(shm_key, (MAX_ORDERS + 5)*sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) < 0) error("create shmget");
    if((shm = (int*)shmat(shm_desc, NULL, 0)) < 0) error("shmat");
    for (int i = 0; i < 5; i++) shm[i] = 0;
    for (int i = 5; i < MAX_ORDERS + 5; i++) shm[i] = -1;
}

void get_shm(){
    key_t shm_key = ftok("/tmp",0x999);
    if((shm_desc = shmget(shm_key, 0, 0)) < 0) error("get shmget");
    if((shm = (int*)shmat(shm_desc, NULL, 0)) < 0) error("shmat");
}

void close_ipc(){
    if(shmdt((void*)shm) < 0) error("shmdt");
}

void remove_shm(){
    if(shmctl(shm_desc, IPC_RMID, NULL) < 0) error("remove shmctl");
}

void mxn_get_sem(action action_m, action action_x, action action_n){
    int nops = 1, i = 0;
    nops += action_n == SET ? 1 : 0;
    nops += action_x % 2 == 0 ? 1 : 0;
    nops += action_m % 2 == 0 ? 1 : 0;
    struct sembuf* get = 
        (struct sembuf*) calloc(nops,sizeof(struct sembuf));
    get[i].sem_num = 1;
    get[i].sem_flg = SEM_UNDO;
    get[i++].sem_op = -1;
    if(action_n == SET) {
        get[i].sem_num = 0;
        get[i].sem_flg = SEM_UNDO;
        get[i++].sem_op = -1;
    }
    if(action_x % 2 == 0) {
        get[i].sem_num = 2;
        get[i].sem_flg = SEM_UNDO;
        get[i++].sem_op = 5 - action_x;
    }
    if(action_m % 2 == 0) {
        get[i].sem_num = 3;
        get[i].sem_flg = SEM_UNDO;
        get[i++].sem_op = 5 - action_m;
    }
    if(semop(sem_desc, get, nops) < 0) error("get semop");
}

void mxn_return_sem(action action_x){
    int nops = action_x == DEC ? 2 : 1;
    struct sembuf* get = 
        (struct sembuf*) calloc(nops,sizeof(struct sembuf));
    get[0].sem_num = 1;
    get[0].sem_flg = SEM_UNDO;
    get[0].sem_op = 1;
    if(action_x == DEC) {
        get[1].sem_num = 0;
        get[1].sem_flg = SEM_UNDO;
        get[1].sem_op = 1;
    }
    if(semop(sem_desc, get, nops) < 0) error("return semop");
}