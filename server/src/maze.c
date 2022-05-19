#include "maze.h"



int maze[MAZE_H][MAZE_W];


// in the maze walls are represented by -1, 
//paths are -2, ghost -3 and the players are their ids 

//function that parse the maze from the file and put in a 2d array 0 are -2 and 1 are -1
int parse_maze(char *file_name) {
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
            if (c == '\n') {
                j--;
                continue;
            }
            if (c == '0') {
                maze[i][j] = -2;
            } else if (c == '1') {
                maze[i][j] = -1;
            }
        }
    }
    fclose(fp);
    return 0;
}    


//function that put a player_id randomly in the maze
int put_player_id(int player_id){
    int x, y;
    int count = 0;
    while(count < 1){
        x = rand() % MAZE_W;
        y = rand() % MAZE_H;
        if(maze[y][x] == -2){
            maze[y][x] = player_id;
            count++;
        }
    }
    return 0;
}

//function that put n ghosst as -3 randomly in the maze
int put_ghosts(int n){
    int x, y;
    int count = 0;
    while(count < n){
        x = rand() % MAZE_W;
        y = rand() % MAZE_H;
        if(maze[y][x] == -2){
            maze[y][x] = -3;
            count++;
        }
    }
    return 0;
}


//function move_up to move the player_id up if possible
int move_up(int player_id){
    int x, y;
    for(y = 0; y < MAZE_H; y++){
        for(x = 0; x < MAZE_W; x++){
            if(maze[y][x] == player_id){
                if(maze[y-1][x] == -2){
                    maze[y][x] = -2;
                    maze[y-1][x] = player_id;
                    return 0;
                }
            }
        }
    }
    return -1;
}
//function move_down to move the player_id down if possible
int move_down(int player_id){
    int x, y;
    for(y = 0; y < MAZE_H; y++){
        for(x = 0; x < MAZE_W; x++){
            if(maze[y][x] == player_id){
                if(maze[y+1][x] == -2){
                    maze[y][x] = -2;
                    maze[y+1][x] = player_id;
                    return 0;
                }
            }
        }
    }
    return -1;
}
//function move_left to move the player_id left if possible
int move_left(int player_id){
    int x, y;
    for(y = 0; y < MAZE_H; y++){
        for(x = 0; x < MAZE_W; x++){
            if(maze[y][x] == player_id){
                if(maze[y][x-1] == -2){
                    maze[y][x] = -2;
                    maze[y][x-1] = player_id;
                    return 0;
                }
            }
        }
    }
    return -1;
}
//function move_right to move the player_id right if possible
int move_right(int player_id){
    int x, y;
    for(y = 0; y < MAZE_H; y++){
        for(x = 0; x < MAZE_W; x++){
            if(maze[y][x] == player_id){
                if(maze[y][x+1] == -2){
                    maze[y][x] = -2;
                    maze[y][x+1] = player_id;
                    return 0;
                }
            }
        }
    }
    return -1;
}


//main function that print the maze
void print_maze(){
    int i, j;
    for (i = 0; i < MAZE_W; i++) {
        for (j = 0; j < MAZE_H; j++) {
            printf("%d", maze[i][j]);
        }
        printf("\n");
    }
}


