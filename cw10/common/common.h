#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_CLIENTS 20

void error(char* msg);

int make_sockaddr(struct sockaddr *sa, socklen_t *len, const char *name,int domain, int will_bind);


