#include "in-game.h"

int last_digit_ip = 0;

// Need to free ip after use
char *get_free_ip() {
    char *ip = malloc(16);
    sprintf(ip, "224.1.1.%d######", last_digit_ip++);
    return ip;
}

int greet_player(PlayerInfo info) {
    //
    Game *game = get_game(info.game_id);
    char *ip = get_free_ip();
    // Generate random number between 1000 and 9999
    int portGame = rand() % 10000 + 1000;
    char port[5];
    sprintf(port, "%d", portGame);
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
        }
    }
    free(ip);

    return 0;
}

int ig_interact(int sockfd, PlayerInfo info) {
    //
    greet_player(info);

    return 0;
}