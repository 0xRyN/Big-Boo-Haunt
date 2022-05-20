#ifndef __IN_GAME_H__
#define __IN_GAME_H__

#include "game.h"
#include "multicast.h"
#include "recieve.h"
#include "send.h"
#include "maze.h"

int greet_player(PlayerInfo info);
int ig_interact(int sockfd, PlayerInfo info, int increment_result);
int init_maze(Game *game, int nb_ghosts);


#endif