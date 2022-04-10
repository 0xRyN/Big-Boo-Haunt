#ifndef __RECEIVE_H__
#define __RECEIVE_H__

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int safe_receive(int sockfd, char *buffer, int buffer_size);

#endif