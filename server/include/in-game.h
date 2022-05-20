#ifndef __IN_GAME_H__
#define __IN_GAME_H__

#include "game.h"
#include "recieve.h"
#include "send.h"

int greet_player(PlayerInfo info);
int ig_interact(int sockfd, PlayerInfo info, int increment_result);

#endif