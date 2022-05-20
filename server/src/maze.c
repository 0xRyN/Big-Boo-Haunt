#include "maze.h"

// in the maze walls are represented by -1,
// paths are -2, ghost -3 and the players are their ids

// function that parse the maze from the file and put in a 2d array 0 are -2 and
// 1 are -1
int parse_maze(char *file_name, Maze *maze) {
    FILE *fp;
    int i, j;
    char c;
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return -1;
    }
    for (i = 0; i < MAZE_H; i++) {
        for (j = 0; j < MAZE_W; j++) {
            c = fgetc(fp);
            // printf("%c", c);
            if (c == '\n') {
                j--;
                continue;
            }
            if (c == '0') {
                maze->grid[i][j] = -2;
            } else if (c == '1') {
                maze->grid[i][j] = -1;
            }
        }
    }
    fclose(fp);
    // print_maze(maze);
    return 0;
}

// function that put a player_id randomly in the maze
int **put_player_id(int player_id, Maze *maze) {
    srand(time(NULL));
    maze->nb_players = maze->nb_players + 1;
    int x, y;
    int count = 0;
    while (count < 1) {
        x = rand() % MAZE_W;
        y = rand() % MAZE_H;
        if (maze->grid[y][x] == -2) {
            maze->grid[y][x] = player_id;
            count++;
        }
    }
    int **pos = malloc(2 * sizeof(int *));
    pos[0] = malloc(sizeof(int));
    pos[1] = malloc(sizeof(int));
    *(pos[0]) = x;
    *(pos[1]) = y;
    return pos;
}

// function that put n ghost as -3 randomly in the maze
int put_ghosts(int n, Maze *maze) {
    srand(time(NULL));
    maze->nb_ghost = n;
    int x, y;
    int count = 0;
    while (count < n) {
        x = rand() % MAZE_W;
        y = rand() % MAZE_H;
        if (maze->grid[y][x] == -2) {
            maze->grid[y][x] = -3;
            count++;
        }
    }
    return 0;
}

// function move_up to move the player_id up if possible
int move_up(int player_id, Maze maze) {
    int x, y;
    for (y = 0; y < MAZE_H; y++) {
        for (x = 0; x < MAZE_W; x++) {
            if (maze.grid[y][x] == player_id) {
                // here we check if the player hit a ghost
                // if he hit on the ghost disappear
                // and the function return 2
                if(y-1 < 0){
                    return -1;
                }else if (maze.grid[y - 1][x] == -3) {
                    maze.grid[y][x] = -2;
                    maze.grid[y - 1][x] = player_id;
                    return 2;
                } else if (maze.grid[y - 1][x] == -2) {
                    maze.grid[y][x] = -2;
                    maze.grid[y - 1][x] = player_id;
                    return 0;
                }
            }
        }
    }
    return -1;
}
// function move_down to move the player_id down if possible
int move_down(int player_id, Maze maze) {
    int x, y;
    for (y = 0; y < MAZE_H; y++) {
        for (x = 0; x < MAZE_W; x++) {
            // check if it's -3 and if it's -3 we return 2
            if(y+1 >= MAZE_H){
                return -1;
            }else if (maze.grid[y + 1][x] == -3) {
                maze.grid[y][x] = -2;
                maze.grid[y + 1][x] = player_id;
                return 2;
            } else if (maze.grid[y][x] == player_id) {
                if (maze.grid[y + 1][x] == -2) {
                    maze.grid[y][x] = -2;
                    maze.grid[y + 1][x] = player_id;
                    return 0;
                }
            }
        }
    }
    return -1;
}
// function move_left to move the player_id left if possible
int move_left(int player_id, Maze maze) {
    int x, y;
    for (y = 0; y < MAZE_H; y++) {
        for (x = 0; x < MAZE_W; x++) {
            if (maze.grid[y][x] == player_id) {
                if(x-1 < 0){
                    return -1;
                }else if (maze.grid[y][x - 1] == -3) {
                    maze.grid[y][x] = -2;
                    maze.grid[y][x - 1] = player_id;
                    return 2;
                } else if (maze.grid[y][x - 1] == -2) {
                    maze.grid[y][x] = -2;
                    maze.grid[y][x - 1] = player_id;
                    return 0;
                }
            }
        }
    }
    return -1;
}
// function move_right to move the player_id right if possible
int move_right(int player_id, Maze maze) {
    int x, y;
    for (y = 0; y < MAZE_H; y++) {
        for (x = 0; x < MAZE_W; x++) {
            if (maze.grid[y][x] == player_id) {
                if(x+1>=MAZE_W){
                    return -1;
                }else if (maze.grid[y][x + 1] == -3) {
                    maze.grid[y][x] = -2;
                    maze.grid[y][x + 1] = player_id;
                    return 2;
                } else if (maze.grid[y][x + 1] == -2) {
                    maze.grid[y][x] = -2;
                    maze.grid[y][x + 1] = player_id;
                    return 0;
                }
            }
        }
    }
    return -1;
}

// main function that print the maze
void print_maze(Maze maze) {
    int i, j;
    for (i = 0; i < MAZE_W; i++) {
        for (j = 0; j < MAZE_H; j++) {
            if (maze.grid[j][i] == -2) {
                printf("  ");
            } else if (maze.grid[j][i] == -1) {
                printf("X ");
            } else if (maze.grid[j][i] == -3) {
                printf("G ");
            } else {
                printf("%d ", maze.grid[j][i]);
            }
            // printf("%d", maze.grid[i][j]);
        }
        printf("\n");
    }
}
