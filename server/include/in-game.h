#ifndef __IN_GAME_H__
#define __IN_GAME_H__

#include "recieve.h"
#include "send.h"
#include "game.h"

int greet_player(PlayerInfo info);
int ig_interact(int sockfd, PlayerInfo info);

#endif