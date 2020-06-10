#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

int counter = 0;
pid_t catcher = -1;
void (*send)(int);
int sig1,sig2,confirmation = 0;


void handler(int sig, siginfo_t *info, void *ucontext){
    if(sig == sig1){
        if(info->si_value.sival_int)
            printf("sender received %d-th sig1\n",info->si_value.sival_int);
        if(!confirmation) counter++;
    } else if(sig == sig2){
        printf("sender: %d sig1\n",counter);
        counter = -1;
        exit(0);
    }
    confirmation = 0;
    return;
}
void send_kill(int num){
    for (int i = 0; i < num; i++){
        sigset_t new_mask;
        sigfillset(&new_mask);
        sigdelset(&new_mask,SIGUSR1);
        confirmation = 1;
        kill(catcher,sig1);
        sigsuspend(&new_mask);
    }
    kill(catcher,sig2);
}

void send_queue(int num){
    union sigval number;
    for (int i = 0; i < num; i++){
        number.sival_int = i+1;
        sigset_t new_mask;
        sigfillset(&new_mask);
        sigdelset(&new_mask,SIGUSR1);
        confirmation = 1;
        sigqueue(catcher,sig1,number);
        sigsuspend(&new_mask);
    }
    number.sival_int = -1;
    sigqueue(catcher,sig2,number);
}

int main(int argc, char** argv){
    if(argc != 4) return 22;
    catcher = atoi(argv[1]);
    int num = atoi(argv[2]);
    if(strcmp(argv[3],"KILL") == 0){
        send = send_kill;
        sig1 = SIGUSR1;
        sig2 = SIGUSR2;
    } else if (strcmp(argv[3],"SIGQUEUE") == 0){
        send = send_queue;
        sig1 = SIGUSR1;
        sig2 = SIGUSR2;
    } else if (strcmp(argv[3],"SIGRT") == 0){
        send = send_kill;
        sig1 = SIGRTMIN;
        sig2 = SIGRTMIN+1;
    } else return 22;
    struct sigaction new_action;
    new_action.sa_sigaction = handler;
    sigfillset(&new_action.sa_mask);
    new_action.sa_flags = SA_SIGINFO;
    if(sigaction(sig1, &new_action, NULL) < 0)
        printf("can't catch %d\n",sig1); 
    if(sigaction(sig2, &new_action, NULL) < 0)
        printf("can't catch %d\n",sig2); 
    if(sigaction(SIGUSR1, &new_action, NULL) < 0)
        printf("can't catch SIGUSR1\n"); 
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask,sig1);
    sigdelset(&mask,sig2);
    sigprocmask(SIG_SETMASK,&mask,NULL);
    send(num);
    while(counter >= 0){
        pause();
    }
    return 0;
}