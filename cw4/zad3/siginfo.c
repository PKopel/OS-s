#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

void handler(int sig, siginfo_t *info, void *ucontext){
    if(sig == SIGCHLD){
        printf("otrzymano SIGCHLD\npid potomka: %d\nrzeczywiste uid: %d\nstatus wyjścia: %d\nczas systemowy: %ld\n",
        info->si_pid,info->si_uid,info->si_status,info->si_stime);
    } else if(sig == SIGALRM){
        printf("otrzymano SIGALRM\npid wysyłającego: %d\nid timera: %d\nczas od wysłania: %d\nczas urzytkownika: %ld\n",
        info->si_pid,info->si_timerid,info->si_overrun,info->si_utime);
    } else if(sig == SIGTSTP){
        printf("otrzymano SIGTSTP\npid wysyłającego: %d\nrzeczywiste uid: %d\nczas systemowy: %ld\nczas urzytkownika: %ld\n",
        info->si_pid,info->si_uid,info->si_stime,info->si_utime);
    }   
    return;
}

int main(void){
    struct sigaction new_action;
    new_action.sa_sigaction = handler;
    sigfillset(&new_action.sa_mask);
    new_action.sa_flags = SA_SIGINFO;
    if(sigaction(SIGCHLD, &new_action, NULL) < 0)
        printf("can't catch SIGCHLD\n"); 
    if(sigaction(SIGALRM, &new_action, NULL) < 0)
        printf("can't catch SIGALRM\n"); 
    if(sigaction(SIGTSTP, &new_action, NULL) < 0)
        printf("can't catch SIGTSTP\n"); 
    pid_t child = fork();
    if (child == 0)
        exit(10);
    alarm(2);
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask,SIGALRM);
    sigsuspend(&mask);
    printf("press CTRL+Z\n");
    sigfillset(&mask);
    sigdelset(&mask,SIGTSTP);
    sigsuspend(&mask);
    return 0;
}