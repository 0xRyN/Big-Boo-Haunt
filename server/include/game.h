#ifndef __GAME_H__
#define __GAME_H__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 16
#define MAX_PLAYERS 100
#define MAX_GAMES 10

typedef struct Game {
    int id;
    int player_count;
    char players[MAX_PLAYERS][MAX_NAME + 1];
    char player_sockets[MAX_PLAYERS];
    int port;
} Game;

int create_game(char *player, int socket, int port);
int join_game(int id, int socket, int port, char *player);
int destroy_game(int id);
void print_games();

#endif