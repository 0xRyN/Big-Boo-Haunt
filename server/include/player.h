#ifndef __PLAYER_H__
#define __PLAYER_H__

#define MAX_NAME 8
#define MAX_PLAYERS 10
#define MAX_GAMES 10
#define MIN_PLAYERS 0

typedef struct Player {
    char id[MAX_NAME + 1];
    int socket;
    int port;
    int is_ready;
    int x;
    int y;
    int score;
} Player;

#endif