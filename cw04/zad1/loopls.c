#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int running = 1;

void TSTP_handle(int signum){
    if(running){
        printf("Oczekuję na CTRL+Z - kontynuacja albo CTR+C - zakończenie programu\n");
        running = 0;
        sigset_t suspend;
        sigfillset(&suspend);
        sigdelset(&suspend, SIGINT);
        sigdelset(&suspend, SIGTSTP);
        sigsuspend(&suspend);
    } else running = 1;
}

void INT_handle(int signum){
    printf("Odebrano sygnał SIGINT\n");
    exit(0);
}

int main() {
    struct sigaction new_action;
    new_action.sa_handler = TSTP_handle;
    sigfillset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    if(sigaction(SIGTSTP, &new_action, NULL) < 0)
        printf("can't catch SIGTSTP");  
    if(signal(SIGINT, INT_handle) == SIG_ERR)
        printf("can't catch SIGINT");
    while(1){
        system("ls");
        sleep(1);
    }
    return 0;
}