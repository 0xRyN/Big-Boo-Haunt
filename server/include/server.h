#ifndef __SERVER_H__
#define __SERVER_H__

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "game.h"
#include "lobby.h"
#include "thread-args.h"

#define PORT "8080"

#endif
