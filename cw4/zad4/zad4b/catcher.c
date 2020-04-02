#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

int counter = 0;
pid_t sender = -1;
void (*send)(int,int);


void send_kill(int sig1, int sig2){
    for (int i = 0; i < counter; i++){
        kill(sender,sig1);
    }
    kill(sender,sig2);
}

void send_queue(int sig1, int sig2){
    union sigval number;
    for (int i = 0; i < counter; i++){
        number.sival_int = i+1;
        sigqueue(sender,sig1,number);
    }
    number.sival_int = -1;
    sigqueue(sender,sig2,number);
}

void handler(int sig, siginfo_t *info, void *ucontext){
    if(sig == SIGUSR1 || sig == SIGRTMIN){
        sender = info->si_pid;
        counter++;
        if(info->si_value.sival_int){
            union sigval val;
            sigqueue(sender,SIGUSR1,val);
        } else {
            kill(sender,SIGUSR1);
        }
    } else if(sig == SIGUSR2 || sig == SIGRTMIN+1){
        if(info->si_value.sival_int){
            send = send_queue;
        } else {
            send = send_kill;
        }
        sender = info->si_pid;
        if(sig == SIGUSR2){
            send(SIGUSR1,SIGUSR2);
        } else {
            send(SIGRTMIN,SIGRTMIN+1);
        }
        printf("catcher: %d %d\n",counter,sig);
        counter = -1;
        exit(0);
    }   
    return;
}

int main(void){
    printf("%d\n",getpid());
    struct sigaction new_action;
    new_action.sa_sigaction = handler;
    sigfillset(&new_action.sa_mask);
    new_action.sa_flags = SA_SIGINFO;
    if(sigaction(SIGUSR1, &new_action, NULL) < 0)
        printf("can't catch SIGUSR1\n"); 
    if(sigaction(SIGUSR2, &new_action, NULL) < 0)
        printf("can't catch SIGUSR1\n"); 
    if(sigaction(SIGRTMIN, &new_action, NULL) < 0)
        printf("can't catch SIGRTMIN\n"); 
    if(sigaction(SIGRTMIN+1, &new_action, NULL) < 0)
        printf("can't catch SIGRTMIN+1\n"); 
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask,SIGUSR1);
    sigdelset(&mask,SIGUSR2);
    sigdelset(&mask,SIGRTMIN);
    sigdelset(&mask,SIGRTMIN+1);
    sigprocmask(SIG_BLOCK,&mask,NULL);
    while(counter >= 0){
        pause();
    }
    return 0;
}