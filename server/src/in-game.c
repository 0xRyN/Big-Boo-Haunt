#include "in-game.h"

int last_digit_ip = 0;

// Need to free ip after use
char *get_free_ip() {
    char *ip = malloc(16);
    sprintf(ip, "224.1.1.%d######", last_digit_ip);
    return ip;
}

char *get_free_port() {
    char *port = malloc(6);
    sprintf(port, "540%d", last_digit_ip);
    return port;
}

int init_maze(Game *game, int nb_ghosts) {
    parse_maze("Maze/1.txt", &(game->maze));

    // for evry player in game put all the player_id in the maze with
    // put_player_id
    print_maze(game->maze);
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i] != NULL) {
            int **pos = put_player_id(i, &(game->maze));
            game->players[i]->x = *(pos[0]);
            game->players[i]->y = *(pos[1]);
            printf("x: %d\n", game->players[i]->x);
            printf("y: %d\n", game->players[i]->y);
            free(pos[0]);
            free(pos[1]);
            free(pos);
            printf("POSITION X: %d\n", game->players[i]->x);
            printf("POSITION Y: %d\n", game->players[i]->y);
        }
    }
    print_maze(game->maze);
    put_ghosts(nb_ghosts, &(game->maze));
    print_maze(game->maze);

    return 0;
}

int greet_player(PlayerInfo info) {
    //
    Game *game = get_game(info.game_id);
    char *ip = get_free_ip();
    init_maze(game, 5);
    // print_maze(game->maze);
    //  Generate random number between 1000 and 9999
    char *port = get_free_port();
    last_digit_ip++;
    for (int j = 0; j < MAX_PLAYERS; j++) {
        if (game != NULL && game->players[j] != NULL) {
            char buffer[80];
            printf("info.player_id: %d\n", info.player_id);
            printf("mazeHeight: %d\n", game->mazeHeight);
            printf("mazeWidth: %d\n", game->mazeWidth);
            printf("amountOfGhosts: %d\n", game->amountOfGhosts);
            printf("ip: %s\n", ip);
            printf("port: %s\n", port);
            sprintf(buffer, "WELCO %d %hu %hu %d %s %s***", info.game_id,
                    game->mazeHeight, game->mazeWidth, game->amountOfGhosts, ip,
                    port);
            printf("%s***\n", port);
            if (safe_send(game->players[j]->socket, buffer, strlen(buffer)) <
                0) {
                free(ip);
                free(port);
                puts("Error sending greeting");
                return -1;
            }
            char buffer2[80];
            sprintf(buffer2, "POSIT %d %d %d***", info.player_id,
                    game->players[j]->x, game->players[j]->y);
            if (safe_send(game->players[j]->socket, buffer2, strlen(buffer2)) <
                0) {
                puts("Error sending greeting");
                return -1;
            }
        }
    }
    free(ip);
    free(port);

    return 0;
}

int ig_interact(int sockfd, PlayerInfo info, int increment_result) {
    //
    Game *game = get_game(info.game_id);
    if (increment_result == 1) {
        greet_player(info);
    }
    while (1) {
        printf("Waiting for message\n");
        char buffer[100];
        int res = safe_receive(sockfd, buffer, 100);
        if (res < 0) {
            puts("Client disconnected !");
            leave_game(info);
            info.player_id = -1;
            info.game_id = -1;
            return 0;
        }
        int op = parse_operation(buffer);
        if (op < 0) {
            puts("Invalid operation");
            return -1;
        } else {
            if (op == OP_MALLQ) {
                printf("message all :\n");
                multicast_send("224.1.1.0", "5400", buffer);
                char res_buffer[40];

                sprintf(res_buffer, "MALL!***");
                if (safe_send(sockfd, res_buffer, 8) < 0) {
                    puts("Error sending registration result");
                    return -1;
                }
            } else if (op == OP_UPMOV) {
                /* code */
            } else if (op == OP_DOMOV) {
                /* code */
            } else if (op == OP_LEMOV) {
                /* code */
            } else if (op = OP_RIMOV) {
                /* code */
            } else {
                multicast_send("224.1.1.0", "5400",
                               "T'es dans ma partie ou quoi lol!! XD");
                char res_buffer[40];
                sprintf(res_buffer, "REGOK***");
                if (safe_send(sockfd, res_buffer, 8) < 0) {
                    puts("Error sending registration result");
                    return -1;
                }
            }
            printf("ICI\n");
        }
    }
}