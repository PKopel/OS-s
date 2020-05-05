#include "common.h"
#include <signal.h>

void cleanup(){
    remove_sem();
    remove_shm();
}

void handle_sigint(int signum){
    cleanup();
}

int main(int argc, char** argv){
    if(argc != 4) return 22;
    int receivers = atoi(argv[1]);
    int packers = atoi(argv[2]);
    int senders = atoi(argv[3]);
    if(atexit(cleanup) < 0) error("atexit");
    if(signal(SIGINT, handle_sigint) == SIG_ERR) error("signal");
    create_sem();
    create_shm();
    pid_t child_pid;
    for (int i = 0; i < receivers; i++)
    {
        if((child_pid = fork()) < 0) error("fork");
        if (child_pid == 0){
            execlp("./receiver.o","receiver.o", NULL);
            exit(0);
        }
    }
    for (int i = 0; i < packers; i++)
    {
        if((child_pid = fork()) < 0) error("fork");
        if (child_pid == 0){
            execlp("./packer.o","packer.o", NULL);
            exit(0);
        }
    }
    for (int i = 0; i < senders; i++)
    {
        if((child_pid = fork()) < 0) error("fork");
        if (child_pid == 0) {
            execlp("./sender.o","sender.o", NULL);
            exit(0);
        }
    }
    pause();
}