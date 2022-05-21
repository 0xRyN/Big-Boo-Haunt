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
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i] != NULL) {
            int **pos = put_player_id(i, &(game->maze));
            game->players[i]->x = *(pos[0]);
            game->players[i]->y = *(pos[1]);

            free(pos[0]);
            free(pos[1]);
            free(pos);
        }
    }
    put_ghosts(nb_ghosts, &(game->maze));

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
            sprintf(buffer, "WELCO %d %hu %hu %d %s %s***", info.game_id,
                    game->mazeHeight, game->mazeWidth, game->amountOfGhosts, ip,
                    port);
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

// Send player position to player after move.
// If position is 1 digit, add 00 before.
// If position is 2 digits, add 0 before.

int send_player_position(PlayerInfo info, int return_val) {
    Game *game = get_game(info.game_id);
    char buffer[80];
    char pos_x[3 + 1];
    char pos_y[3 + 1];

    sprintf(pos_x, "%d", game->players[info.player_id]->x);
    sprintf(pos_y, "%d", game->players[info.player_id]->y);

    // If position is 1 digit, add 00 before.
    if (game->players[info.player_id]->x < 10) {
        sprintf(pos_x, "00%d", game->players[info.player_id]->x);
    }
    if (game->players[info.player_id]->y < 10) {
        sprintf(pos_y, "00%d", game->players[info.player_id]->y);
    }
    // If position is 2 digits, add 0 before.
    if (game->players[info.player_id]->x < 100 &&
        game->players[info.player_id]->x >= 10) {
        sprintf(pos_x, "0%d", game->players[info.player_id]->x);
    }
    if (game->players[info.player_id]->y < 100 &&
        game->players[info.player_id]->y >= 10) {
        sprintf(pos_y, "0%d", game->players[info.player_id]->y);
    }

    // Send position to player
    if (return_val == 2) {
        sprintf(buffer, "MOVEF %s %s***", pos_x, pos_y);
    }

    else {
        sprintf(buffer, "MOVE! %s %s***", pos_x, pos_y);
    }

    if (safe_send(game->players[info.player_id]->socket, buffer,
                  strlen(buffer)) < 0) {
        puts("Error sending position");
        return -1;
    }
    return return_val;
}

int ig_interact(int sockfd, PlayerInfo info, int increment_result) {
    //
    Game *game = get_game(info.game_id);
    if (increment_result == 1) {
        greet_player(info);
    }
    while (1) {
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
        }

        else {
            if (op == OP_MALLQ) {
                printf("message all :\n");
                multicast_send("224.1.1.0", "5400", buffer);
                char res_buffer[40];

                sprintf(res_buffer, "MALL!***");
                if (safe_send(sockfd, res_buffer, 8) < 0) {
                    puts("Error sending registration result");
                    return -1;
                }
            }

            else if (op == OP_UPMOV) {
                int return_val = 0;
                struct PLMOV plmov;
                plmov = parse_plmov(buffer);
                int to_move = atoi(plmov.distance);
                for (int i = 0; i < to_move; i++) {
                    int move_res = move_up(info.player_id, &(game->maze));

                    if (move_res == -1) {
                        puts("Error moving up - colliding a wall");
                        break;
                    }

                    else if (move_res == -2) {
                        puts("Colliding a ghost");
                        return_val = 2;
                    }
                }

                send_player_position(info, return_val);
            }

            else if (op == OP_DOMOV) {
                int return_val = 0;
                struct PLMOV plmov;
                plmov = parse_plmov(buffer);
                int to_move = atoi(plmov.distance);
                for (int i = 0; i < to_move; i++) {
                    int move_res = move_down(info.player_id, &(game->maze));

                    if (move_res == -1) {
                        puts("Error moving up - colliding a wall");
                        break;
                    }

                    else if (move_res == -2) {
                        puts("Colliding a ghost");
                        return_val = 2;
                    }
                }

                send_player_position(info, return_val);
            }

            else if (op == OP_LEMOV) {
                int return_val = 0;
                struct PLMOV plmov;
                plmov = parse_plmov(buffer);
                int to_move = atoi(plmov.distance);
                for (int i = 0; i < to_move; i++) {
                    int move_res = move_left(info.player_id, &(game->maze));

                    if (move_res == -1) {
                        puts("Error moving up - colliding a wall");
                        break;
                    }

                    else if (move_res == -2) {
                        puts("Colliding a ghost");
                        return_val = 2;
                    }
                }

                send_player_position(info, return_val);
            }

            else if (op == OP_RIMOV) {
                int return_val = 0;
                struct PLMOV plmov;
                plmov = parse_plmov(buffer);
                int to_move = atoi(plmov.distance);
                for (int i = 0; i < to_move; i++) {
                    int move_res = move_right(info.player_id, &(game->maze));

                    if (move_res == -1) {
                        puts("Error moving up - colliding a wall");
                        break;
                    }

                    else if (move_res == -2) {
                        puts("Colliding a ghost");
                        return_val = 2;
                    }
                }

                send_player_position(info, return_val);
            } else if (op == OP_SENDQ) {
                char resbuffer[200];
                struct SENDQ msgparse;
                msgparse = parse_sendq(buffer);
                char repbuffer[8];
                int is_here = 0;
                char port[4];
                char ip[15];
                for (int i = 0; i < MAX_PLAYERS; i++) {
                    printf("%s ET %s\n", game->players[i]->id, msgparse.id);
                    if (game->players[i] != NULL) {
                        printf("port : %s\n", game->players[i]->port);
                        if (strcmp(game->players[i]->id, msgparse.id) == 0) {
                            is_here = 1;
                            // printf("port : %s\n", game->players[i]->port);
                            sprintf(port, "%s", game->players[i]->port);
                            // printf("ip :
                            // %s\n",inet_ntoa(game->players[i]->addr->sin_addr));
                            sprintf(
                                ip, "%s",
                                inet_ntoa(game->players[i]->addr->sin_addr));
                            break;
                        }
                    }
                }
                // printf("%s\n", msgparse.message);
                if (is_here == 1 &&
                    send_udp(ip, port, msgparse.message) != -1) {
                    sprintf(resbuffer, "SEND!***");
                    if (safe_send(sockfd, resbuffer, strlen(resbuffer)) < 0) {
                        puts("Error sending message");
                        return -1;
                    }
                } else {
                    sprintf(resbuffer, "NSEND***");
                    if (safe_send(sockfd, resbuffer, strlen(resbuffer)) < 0) {
                        puts("Error sending message");
                        return -1;
                    }
                }
            }

            else if (op == OP_GLISQ) {
                int game_id = info.game_id;
                char firstres[12];
                uint8_t amount_players = game->amout_of_ready_players;
                sprintf(firstres, "GLIS!%hu***", amount_players);
                if (safe_send(sockfd, firstres, strlen(firstres)) < 0) {
                    puts("Error sending message");
                    return -1;
                }
                for (int i = 0; i < MAX_PLAYERS; i++) {
                    if (game->players[i] != NULL) {
                        char resbuffer[200];
                        sprintf(resbuffer, "GLIS!%s***", game->players[i]->id);
                        if (safe_send(sockfd, resbuffer, strlen(resbuffer)) <
                            0) {
                            puts("Error sending message");
                            return -1;
                        }
                    }
                }
            }

            else {
                multicast_send("224.1.1.0", "5400",
                               "T'es dans ma partie ou quoi lol!! XD");
                char res_buffer[40];
                sprintf(res_buffer, "REGOK***");
                if (safe_send(sockfd, res_buffer, 8) < 0) {
                    puts("Error sending registration result");
                    return -1;
                }
            }
        }
    }
}