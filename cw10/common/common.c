#include "common.h"


void error(char* msg){
    perror(msg);
    exit(errno);
}

int make_sockaddr(struct sockaddr *sa, socklen_t *len, const char *name,int domain, int will_bind){
    struct addrinfo *infop = NULL;
    if (domain == AF_UNIX) {
        struct sockaddr_un *sunp = (struct sockaddr_un *)sa;
        if (strlen(name) >= sizeof(sunp->sun_path)) {
            errno = ENAMETOOLONG;
            error("AF_UNIX name too long");
        }
        strcpy(sunp->sun_path, name);
        sunp->sun_family = AF_UNIX;
        *len = sizeof(*sunp);
    } else {
        struct addrinfo hint;
        char nodename[200], *servicename;
        memset(&hint, 0, sizeof(hint));
        hint.ai_family = domain;
        hint.ai_socktype = SOCK_STREAM;
        if (will_bind) hint.ai_flags = AI_PASSIVE;
        strcpy(nodename, name);
        servicename = strchr(nodename, ':');
        if (servicename == NULL) {
            errno = EINVAL;
            error("AF_INET wrong name");
        }
        *servicename = '\0';
        servicename++;
        int ret;
        if( (ret = getaddrinfo(nodename, servicename, &hint, &infop)) != 0 ){
            printf("%s\n", gai_strerror(ret));
            exit(EXIT_FAILURE);
        };
        memcpy(sa, infop->ai_addr, infop->ai_addrlen);
        *len = infop->ai_addrlen;
        freeaddrinfo(infop);
    }
    return 0;
}

