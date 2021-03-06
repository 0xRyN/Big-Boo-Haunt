#ifndef __MAZE_H__
#define __MAZE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAZE_H 30
#define MAZE_W 30

typedef struct Maze {
    int grid[MAZE_H][MAZE_W];
    int nb_ghost;
    int nb_players;
} Maze;

int parse_maze(char* file_name, Maze* maze);
int** put_player_id(int player_id, Maze* maze);
int put_ghosts(int n, Maze* maze);
int move_up(int player_id, Maze* maze, int x, int y);
int move_down(int player_id, Maze* maze, int x, int y);
int move_left(int player_id, Maze* maze, int x, int y);
int move_right(int player_id, Maze* maze, int x, int y);

int move_up_ghost(Maze* maze, int x, int y);
int move_down_ghos(Maze* maze, int x, int y);
int move_left_ghos(Maze* maze, int x, int y);
int move_right_ghos(Maze* maze, int x , int y);
int move_ghosts(Maze* maze);

void print_maze(Maze maze);

#endif