#ifndef __THREAD_ARGS_H__
#define __THREAD_ARGS_H__

typedef struct Thread_Args {
    int sockfd;
    struct sockaddr_in client_addr;
} Thread_Args;

#endif