#include "game.h"

int num_games = 0;
int game_status[MAX_GAMES];
Game* games[MAX_GAMES];
pthread_mutex_t game_mutex = PTHREAD_MUTEX_INITIALIZER;

// Will get the smallest ID that is not used
int get_game_id() {
    // State could be changing in for loop, so lock
    pthread_mutex_lock(&game_mutex);
    for (int i = 0; i < MAX_GAMES; i++) {
        if (game_status[i] == 0) {
            game_status[i] = 1;
            pthread_mutex_unlock(&game_mutex);
            return i;
        }
    }
    return -1;
}

int create_game(char* player, int socket, int port) {
    // Get the smallest id for the new game
    int id = get_game_id();

    // We are changing values, therefore we need to lock the mutex
    pthread_mutex_lock(&game_mutex);
    // Fill in the Game struct
    games[id] = malloc(sizeof(Game));
    Game* cur = games[id];
    if (cur == NULL) {
        return -1;
    }
    cur->id = id;
    cur->player_ports[0] = port;
    cur->player_count = 1;
    strcpy(cur->players[0], player);
    cur->player_sockets[0] = socket;

    // Fill in the other players' sockets to -1 (no one is connected)
    for (int i = 0; i < MAX_PLAYERS; i++) {
        cur->player_sockets[i] = -1;
    }
    // We finished modifying values
    pthread_mutex_unlock(&game_mutex);

    return 0;
}

int join_game(int id, int socket, int port, char* player) {
    puts("hello");
    // Check if the game exists
    if (id < 0 || id >= MAX_GAMES) {
        puts("Game does not exist (ID out of bounds)");
        return -1;
    }
    // We are checking different values of the struct, therefore we need to lock
    // the mutex
    pthread_mutex_lock(&game_mutex);
    Game* cur = games[id];
    if (cur == NULL) {
        puts("Game does not exist (ID not allocated)");
        return -1;
    }

    // Check if the game is full
    if (cur->player_count >= MAX_PLAYERS) {
        puts("Game is full");
        return -1;
    }

    // Add the player to the game
    int player_id = -1;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (cur->players[i] == NULL) {
            player_id = i;
            break;
        }
    }
    if (player_id == -1) {
        puts("Could not find free slot in game");
        return -1;
    }
    strcpy(cur->players[id], player);
    cur->player_count++;
    cur->player_sockets[id] = socket;
    cur->player_ports[id] = port;

    // We finished checking / modifying values
    pthread_mutex_unlock(&game_mutex);
    return 0;
}

int destroy_game(int id) {
    puts("hello");
    // Check if the game exists
    if (id < 0 || id >= MAX_GAMES) {
        puts("Game does not exist (ID out of bounds)");
        return -1;
    }
    // We are changing values, therefore we need to lock the mutex
    pthread_mutex_lock(&game_mutex);

    game_status[id] = 0;
    free(games[id]);

    // We finished modifying values
    pthread_mutex_unlock(&game_mutex);
    return 0;
}

// Function to send games to the client
int send_games(int sockfd) {
    // TODO: the games

    // First, send number of games
    char num_games_str[11];
    uint8_t num_games_int = num_games;
    snprintf(num_games_str, 11, "GAMES %c***", num_games_int);
    if (safe_send(sockfd, num_games_str, 10) < 0) {
        puts("Error sending number of games");
        return -1;
    }

    // Then, send each game
    for (int i = 0; i < MAX_GAMES; i++) {
        // Check if game exists (status = 1)
        if (game_status[i] == 1) {
            char game_str[13];
            uint8_t game_id = games[i]->id;
            uint8_t player_count = games[i]->player_count;
            snprintf(game_str, 13, "OGAME %c %c***", game_id, player_count);
            if (safe_send(sockfd, game_str, 12) < 0) {
                puts("Error sending game");
                return -1;
            }
        }
    }
    return 0;
}

void print_games() {
    // We are checking different values of the struct, therefore we need to lock
    // the mutex
    pthread_mutex_lock(&game_mutex);
    for (int i = 0; i < MAX_GAMES; i++) {
        if (game_status[i] == 1) {
            printf("Game %d: ", i);
            for (int j = 0; j < MAX_PLAYERS; j++) {
                if (games[i]->players[j] != NULL) {
                    printf("%s ", games[i]->players[j]);
                }
            }
            printf("\n");
        }
    }
    // We finished checking / modifying values
    pthread_mutex_unlock(&game_mutex);
}
