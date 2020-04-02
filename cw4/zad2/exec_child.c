#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

FILE* report;

int main(int argc, char** argv){
    if (argc != 3) return 22;
    sigset_t mask;
    sigemptyset(&mask);
    int pending = atoi(argv[2]);
    report = fopen("raport2.txt","a");
    if(!pending){
        fprintf(report,"przed raise w procesie potomnym\n");
        fflush(report);
        raise(SIGUSR1);
        fprintf(report,"po raise w procesie potomnym\n");
        fflush(report);
    }
    sigpending(&mask);
    if(sigismember(&mask,SIGUSR1)){
        fprintf(report,"oczekujący SIGUSR1 widoczny w procesie potomnym\n");
        fflush(report);
    } else {
        fprintf(report,"oczekujący SIGUSR1 nie widoczny w procesie potomnym\n");
        fflush(report);
    }
    fclose(report);
    return 0;
}