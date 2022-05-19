#ifndef __MAZE_H__
#define __MAZE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAZE_H 30
#define MAZE_W 30

typedef struct Maze {
    int maze[MAZE_H][MAZE_W];
    int nb_ghost;
    int nb_players;
} Maze;

int parse_maze(char *file_name, Maze maze);
int put_player_id(int player_id, Maze maze);
int put_ghosts(int n, Maze maze);
int move_up(int player_id, Maze maze);
int move_down(int player_id, Maze maze);
int move_left(int player_id, Maze maze);
int move_right(int player_id, Maze maze);
void print_maze();

#endif