#include "lobby.h"

int op_send_games(int sockfd) {
    // Send games is already built into game.c
    int res = send_games(sockfd);
    if (res < 0) {
        puts("Error sending games to client");
        return -1;
    }
    return 0;
}

int op_join_game(int sockfd, char *player, int port) {
    // Get game ID
    int id;
    if (recv(sockfd, &id, sizeof(int), 0) < 0) {
        puts("Error receiving game ID");
        return -1;
    }

    // Join game
    PlayerInfo res = join_game(id, sockfd, port, player);
    if (res.game_id < 0) {
        puts("Error joining game");
        return -1;
    }
    return 0;
}

int interact(int sockfd) {
    // Define flags for each user
    int has_joined = 0;
    int send_start = 0;
    PlayerInfo info = {.player_id = -1, .game_id = -1};

    // First, we send the games to the client
    if (op_send_games(sockfd) < 0) {
        puts("Error sending the games !");
        return -1;
    }

    // Then, we wait for the client to send us his response

    while (1) {
        // Allocate a buffer
        char buffer[80];

        // Receive the message
        int res = safe_receive(sockfd, buffer, 80);
        if (res < 0) {
            puts("Client disconnected !");
            leave_game(info);
            return 0;
        }

        // Now, we need to parse the reponse, and act accordingly
        int op = parse_operation(buffer);
        if (op < 0) {
            puts("Invalid operation");
            return -1;
        }

        // Only a few operations allowed in lobby
        // NEWPL, REGIS, START, UNREG, SIZE?, LIST?, GAME?
        if (op == OP_REGIS) {
            // puts("REGIS");
            // Register the user into a game
            struct REGIS regis;
            regis = parse_regis(buffer);
            PlayerInfo join_result =
                join_game(regis.game_id, sockfd, regis.port, regis.id);
            if (join_result.game_id < 0) {
                puts("Error joining game");
                return -1; //
            }
            info = join_result;

            // We successfully joined the game, so we can send the response
            char res_buffer[40];
            uint8_t int_id = regis.game_id;
            sprintf(res_buffer, "REGOK %hhu", int_id);
            if (safe_send(sockfd, res_buffer, 7) < 0) {
                puts("Error sending registration result");
                return -1;
            }

            has_joined = 1;
        }

        // Create a new game and register the user into it
        else if (op == OP_NEWPL) {
            
            // Check if the player is already in a lobby or in a game
            if(has_joined) {
                // Write an error message to the client
                char res_buffer[40];
                sprintf(res_buffer, "REGNO***");
                if(safe_send(sockfd, res_buffer, 8) < 0) {
                    puts("Error sending registration result");
                    // Stop the connection with the client
                    return -1;
                }
                continue;
            }

            // If the player is not in a lobby, we can create a new game
            struct NEWPL newpl;
            newpl = parse_newpl(buffer);
            // Create a new game
            PlayerInfo create_result = create_game(newpl.id, sockfd, newpl.port);

            // IF the game was not created, we send an error message to the client
            if (create_result.game_id < 0) {
                puts("Error creating game");
                // Stop the connection with the client
                return -1;
            }

            // We successfully created the game, so we can send the response
            info = create_result;
            char res_buffer[40];
            uint8_t int_id = create_result.game_id;
            sprintf(res_buffer, "REGOK %hhu***", int_id);

            // Send the response to the client and if there is an error, stop the connection
            if (safe_send(sockfd, res_buffer, 10) < 0) {
                puts("Error sending registration result");
                return -1;
            }


            has_joined = 1;
        }
        else if (op == OP_LISTQ){
            int send_res = send_game(sockfd, buffer);
            if (send_res < 0) {
                puts("Error sending game");
                return -1;
            }
        }

        else if (op == OP_UNREG) {
            // Unregister the user from the game
            if(has_joined == 0) {
                // Write an error message to the client
                char res_buffer[40];
                sprintf(res_buffer, "DUNNO***");
                if(safe_send(sockfd, res_buffer, 8) < 0) {
                    puts("Error sending registration result");
                    // Stop the connection with the client
                    return -1;
                }
                continue;
            }
            //Leave the game 
            leave_game(info);
            char res_buffer[40];
            uint8_t int_id = info.game_id;
            sprintf(res_buffer, "UNROK %hhu***", int_id);

            if (safe_send(sockfd, res_buffer, 10) < 0) {
                puts("Error sending registration result");
                return -1;
            }
            has_joined = 0;
        }

        else if (op == OP_START) {
            if(has_joined == 0) {
                // Write an error message to the client
                char res_buffer[40];
                sprintf(res_buffer, "DUNNO***");
                if(safe_send(sockfd, res_buffer, 8) < 0) {
                    puts("Error sending registration result");
                    // Stop the connection with the client
                    return -1;
                }
                continue;
            }
            increment_amout_of_ready_players(info);
        }

        else {
            puts("Invalid operation");
            return -1;
        }
    }

    close(sockfd);
    return 0;
}
