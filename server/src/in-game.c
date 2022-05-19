#include "in-game.h"

int greet_player(PlayerInfo info) {
    // 
    Game* game = get_game(info.game_id);
    for (int j = 0; j < MAX_PLAYERS; j++) {
        if (game != NULL && game->players[j] != NULL) {
            char buffer[80];
            sprintf(buffer, "WELCO***");
            if (safe_send(game->players[j]->socket, buffer, strlen(buffer)) < 0) {
                puts("Error sending greeting");
                return -1;
            }
        }
    }


    return 0;
}

int ig_interact(int sockfd, PlayerInfo info) {
    //
    greet_player(info);

    return 0;
}