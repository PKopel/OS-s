#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

typedef struct client_t{
    pthread_t id;
    client_t* next;
} client_t;

typedef struct client_q{
    client_t* head;
    client_t* last;
    client_t* cabinet;
    int aviable;
} client_q;

client_q* clients = NULL;
pthread_mutex_t clients_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clients_cond = PTHREAD_COND_INITIALIZER;

void* client(void* arg){
    client_t* new_client = 
        (client_t*)malloc(sizeof(client_t));
    char* msg = 
        (char*)calloc(40, sizeof(char));
    new_client->id = pthread_self();
    pthread_mutex_lock(&clients_mtx);
    if(clients->cabinet == NULL){
        clients->cabinet = new_client;
        pthread_cond_signal(&clients_cond);
        sprintf(msg,
            "Budze golibrode %ld",
            new_client->id);
    } else if(clients->aviable > 0){
        new_client->next = clients->last;
        clients->last = new_client;
        pthread_cond_signal(&clients_cond);
        sprintf(msg,
            "Poczekalnie, wolne miejsca: %d; %ld",
            --clients->aviable,
            new_client->id);
    } else {
        sprintf(msg,"Zajete; %ld", new_client->id);
    }
    pthread_mutex_unlock(&clients_mtx);
    printf("%s\n", msg);
    free((void*)msg);
}

void* barber(void* arg){
    pthread_mutex_lock(&clients_mtx);
    while (/* condition */)
    {
        /* code */
    }
    
}

int main(int argc, char** argv){

}