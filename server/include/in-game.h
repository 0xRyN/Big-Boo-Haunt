#ifndef __IN_GAME_H__
#define __IN_GAME_H__

#include "recieve.h"
#include "send.h"

int greet_player(int sockfd);
int ig_interact(int sockfd, PlayerInfo info);

#endif