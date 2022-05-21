#ifndef __GAME_H__
#define __GAME_H__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "maze.h"
#include "operations.h"
#include "parser.h"
#include "send.h"

#define MAX_NAME 8
#define MAX_PLAYERS 10
#define MAX_GAMES 10
#define MIN_PLAYERS 0

typedef struct Player {
    char id[MAX_NAME + 1];
    int socket;
    char port[4 + 1];
    int is_ready;
    int x;
    int y;
    struct sockaddr_in *addr;
} Player;
typedef struct Game {
    int id;
    int player_count;
    Player *players[MAX_PLAYERS];
    int amout_of_ready_players;
    int mazeID;
    int mazeWidth;   // PUT INTO U_INT16_T
    int mazeHeight;  // PUT INTO U_INT16_T
    int amountOfGhosts;
    Maze maze;
} Game;
typedef struct PlayerInfo {
    int player_id;
    int game_id;
} PlayerInfo;

int init_games();
PlayerInfo create_game(char *player, int socket, char *port,
                       struct sockaddr_in *addr);
PlayerInfo join_game(int id, int socket, char *port, char *player,
                     struct sockaddr_in *addr);
int leave_game(PlayerInfo info);
int destroy_game(int id);
void print_games();
int send_games(int sockfd);
void print_games();
int send_game(int sockfd, char *buffer);
int ask_size(int sockfd, char *buffer);
int increment_amout_of_ready_players(PlayerInfo info);
Game *get_game(int id);

#endif
