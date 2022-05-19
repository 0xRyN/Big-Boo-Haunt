#include "in-game.h"

int greet_player(int sockfd) {
    char buffer[80];
    sprintf(buffer, "WELCO***");
    if (safe_send(sockfd, buffer, strlen(buffer)) < 0) {
        puts("Error sending greeting");
        return -1;
    }

    return 0;
}

int ig_interact(int sockfd, PlayerInfo info) {
    int greet_return = greet_player(sockfd);
    if (greet_return < 0) {
        puts("Error greeting player");
        return -1;
    }

    return 0;
}