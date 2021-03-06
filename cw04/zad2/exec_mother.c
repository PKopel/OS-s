#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

FILE* report;

int main(int argc, char** argv){
    if (argc != 2) return 22;
    struct sigaction action;
    sigset_t mask;
    sigemptyset(&mask);
    int pending = 0;
    action.sa_flags = 0;
    action.sa_handler = SIG_DFL;
    if(strcmp(argv[1],"ignore") == 0){
        action.sa_handler = SIG_IGN;
    } else if (strcmp(argv[1],"mask") == 0){
        sigaddset(&mask,SIGUSR1);
    } else if (strcmp(argv[1],"pending") == 0){
        pending = 1;
        sigaddset(&mask,SIGUSR1);
    } else return 22;
    sigprocmask(SIG_BLOCK, &mask, NULL);
    sigaction(SIGUSR1,&action,NULL);
    report = fopen("raport2.txt","a");
    fprintf(report,"----------\nexec, opcja %s\n",argv[1]);
    fprintf(report,"przed raise w procesie macierzystym\n");
    fflush(report);
    raise(SIGUSR1);
    fprintf(report,"po raise w procesie macierzystym\n");
    fflush(report);
    sigpending(&mask);
    if(sigismember(&mask,SIGUSR1)){
        fprintf(report,"oczekujący SIGUSR1 widoczny w procesie macierzystym\n");
        fflush(report);
    }
    fclose(report);
    execl("./exec_child.o","exec_child.o",argv[1],pending ? "1" : "0",NULL);
    return 0;
}