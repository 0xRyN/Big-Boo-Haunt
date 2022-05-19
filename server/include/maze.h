#ifndef __MAZE_H__
#define __MAZE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAZE_H 30
#define MAZE_W 30

int parse_maze(char *file_name);
int put_player_id(int player_id);
int put_ghosts(int n);
int move_up(int player_id);
int move_down(int player_id);
int move_left(int player_id);
int move_right(int player_id);
void print_maze();

#endif