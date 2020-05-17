#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

typedef struct client_t{
    pthread_t id;
    struct client_t* next;
} client_t;

typedef struct client_q{
    client_t* head;
    client_t* last;
    client_t* cabinet;
    int clients_waiting;
} client_q;

int max_clients;
client_q* clients;
pthread_mutex_t clients_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clients_cond = PTHREAD_COND_INITIALIZER;


void error(char* msg){
    perror(msg);
    exit(errno);
}

void* client(void* arg){
    client_t* new_client = 
        (client_t*)malloc(sizeof(client_t));
    char* msg = 
        (char*)malloc(40*sizeof(char));
    new_client->id = pthread_self();
    try_again: pthread_mutex_lock(&clients_mtx);
    if(clients->cabinet == NULL && clients->last == NULL){
        clients->cabinet = new_client;
        pthread_cond_signal(&clients_cond);
        printf("Budze golibrode %ld\n",
            new_client->id);
    } else if(clients->clients_waiting < max_clients){
        if(clients->last != NULL) clients->last->next = new_client;
        clients->last = new_client;
        ++clients->clients_waiting;
        if( clients->head == NULL) clients->head = new_client;
        printf("Poczekalnia, wolne miejsca: %d; %ld\n",
            max_clients - clients->clients_waiting,
            new_client->id);
    } else {
        printf("Zajete; %ld\n", new_client->id);
        int sleep_duration = 2+rand()%9;
        pthread_mutex_unlock(&clients_mtx);
        sleep(sleep_duration);
        goto try_again;
    }
    pthread_mutex_unlock(&clients_mtx);
    return NULL;
}

void* barber(void* arg){
    int sleep_duration, number_of_clients = (int)arg;
    while (number_of_clients--){
        pthread_mutex_lock(&clients_mtx);
        while (clients->head == NULL && clients->cabinet == NULL){
            printf("Golibroda: ide spac\n");
            pthread_cond_wait(&clients_cond,&clients_mtx);
        }
        if(clients->cabinet == NULL){
            clients->cabinet = clients->head;
            clients->head = clients->head->next;
            clients->cabinet->next = NULL;
            clients->clients_waiting--;
        }
        printf("Golibroda: czeka %d klientow, gole klienta %ld\n", 
            clients->clients_waiting,
            clients->cabinet->id);
        pthread_mutex_unlock(&clients_mtx);
        sleep_duration = 3+rand()%8;
        sleep(sleep_duration);
        pthread_mutex_lock(&clients_mtx);
        free((void*)clients->cabinet);
        clients->cabinet = NULL;
        pthread_mutex_unlock(&clients_mtx);
    }
    return NULL;
}

int main(int argc, char** argv){
    srand(time(NULL));
    max_clients = atoi(argv[1]);
    int number_of_clients = atoi(argv[2]);
    pthread_t new_thread, barber_id;
    clients = (client_q*)calloc(1,sizeof(client_q));
    if((errno = pthread_create(&barber_id, NULL, barber, (void*)number_of_clients)!= 0)) error("barber create");
    for(int i = 0; i < number_of_clients; i++){
        if((errno = pthread_create(&new_thread, NULL, client, NULL)!= 0)) error("client create");
        sleep(rand()%10);
    }
    pthread_join(barber_id,NULL);
}