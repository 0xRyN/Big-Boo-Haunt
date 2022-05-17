#ifndef __GAME_H__
#define __GAME_H__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "send.h"

#define MAX_NAME 8
#define MAX_PLAYERS 100
#define MAX_GAMES 10

typedef struct Player {
    char id[MAX_NAME + 1];
    int socket;
    int port;
} Player;
typedef struct Game {
    int id;
    int player_count;
    Player *players[MAX_PLAYERS];
} Game;

int init_games();
int create_game(char *player, int socket, int port);
int join_game(int id, int socket, int port, char *player);
int leave_game(int id);
int destroy_game(int id);
void print_games();
int send_games(int sockfd);
void print_games();

#endif
