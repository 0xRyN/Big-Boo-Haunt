#ifndef __IN_GAME_H__
#define __IN_GAME_H__

#include "game.h"
#include "maze.h"
#include "multicast.h"
#include "operations.h"
#include "recieve.h"
#include "send.h"
#include "udpsend.h"
#include "parser.h"

int greet_player(PlayerInfo info);
int ig_interact(int sockfd, PlayerInfo info, int increment_result);
int init_maze(Game *game, int nb_ghosts);

#endif