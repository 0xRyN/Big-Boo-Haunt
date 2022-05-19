#ifndef __LOBBY_H__
#define __LOBBY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "in-game.h"
#include "operations.h"
#include "parser.h"
#include "recieve.h"

int interact(int sockfd);
int op_join_game(int sockfd, char *player, int port);
int op_send_games(int sockfd);

#endif
