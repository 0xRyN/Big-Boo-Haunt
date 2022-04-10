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
    int res = join_game(id, sockfd, port, player);
    if (res < 0) {
        puts("Error joining game");
        return -1;
    }
    return 0;
}

int interact(int sockfd) {
    // First, we send the games to the client
    if (op_send_games(sockfd) < 0) {
        puts("Error sending the games !");
        return -1;
    }

    // Then, we wait for the client to send us his response

    // Allocate a buffer
    char buffer[BUFSIZ];
    if (buffer == NULL) {
        perror("malloc");
        return -1;
    }

    // Receive the message
    int res = safe_receive(sockfd, buffer, BUFSIZ);
    if (res < 0) {
        puts("Error receiving message");
        return -1;
    }

    // Now, we need to parse the reponse, and act accordingly
    int op = parse_operation(buffer);
    if (op < 0) {
        puts("Invalid operation");
        return -1;
    }

    // Only two operations allowed in lobby, NEWPL and REGIS
    if (op == OP_REGIS) {
        // Register the user into a game
        struct REGIS regis;
        regis = parse_regis(buffer);
        int join_result =
            join_game(regis.game_id, sockfd, regis.port, regis.id);
        if (join_result < 0) {
            puts("Error joining game");
            return -1;
        }
    }

    else if (op == OP_NEWPL) {
        // Create a new game and register the user into it
        struct NEWPL newpl;
        newpl = parse_newpl(buffer);
        int create_result = create_game(newpl.id, sockfd, newpl.port);
        if (create_result < 0) {
            puts("Error creating game");
            return -1;
        }
    }

    else {
        puts("Invalid operation");
        return -1;
    }

    return 0;
}