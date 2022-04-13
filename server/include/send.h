#ifndef __SEND_H
#define __SEND_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int safe_send(int sockfd, char *buffer, int buffer_size);

#endif
