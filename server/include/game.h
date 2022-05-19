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
#include "operations.h"
#include "parser.h"

#define MAX_NAME 8
#define MAX_PLAYERS 10
#define MAX_GAMES 10

typedef struct Player {
    char id[MAX_NAME + 1];
    int socket;
    int port;
    int is_ready;
} Player;
typedef struct Game {
    int id;
    int player_count;
    Player *players[MAX_PLAYERS];
    int amout_of_ready_players;
    int labbyID;
    int labbyWidth;
    int labbyHeight;
    int amountOfGhosts;
} Game;
typedef struct PlayerInfo {
    int player_id;
    int game_id;
} PlayerInfo;

int init_games();
PlayerInfo create_game(char *player, int socket, int port);
PlayerInfo join_game(int id, int socket, int port, char *player);
int leave_game(PlayerInfo info);
int destroy_game(int id);
void print_games();
int send_games(int sockfd);
void print_games();
int send_game(int sockfd, char* buffer);

#endif
