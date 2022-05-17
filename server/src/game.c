#include "game.h"

int num_games = 0;
int game_status[MAX_GAMES];
Game* games[MAX_GAMES];
pthread_mutex_t game_mutex;

int init_games() {
    // Init mutex
    pthread_mutex_init(&game_mutex, NULL);

    // Set all games' status to -1 (Not started)
    for (int i = 0; i < MAX_GAMES; i++) {
        game_status[i] = -1;
    }

    return 0;
}

// Will get the smallest ID that is not used
int get_game_id() {
    // State could be changing in for loop, so lock
    pthread_mutex_lock(&game_mutex);
    for (int i = 0; i < MAX_GAMES; i++) {
        if (game_status[i] == -1) {
            game_status[i] = 1;
            pthread_mutex_unlock(&game_mutex);
            return i;
        }
    }
    pthread_mutex_unlock(&game_mutex);
    return -1;
}

// Creates a new game (NEWPL) and adds the player to it
// Will return the game ID if success, -1 otherwise
int create_game(char* player, int socket, int port) {
    printf("Creating game for %s\n", player);
    // Get the smallest id for the new game
    int id = get_game_id();

    // We are changing values, therefore we need to lock the mutex
    pthread_mutex_lock(&game_mutex);

    puts("hey");

    // Increase current game counter
    num_games++;

    // Fill in the Game struct
    games[id] = malloc(sizeof(Game));
    Game* cur = games[id];
    if (cur == NULL) {
        return -1;
    }
    cur->id = id;
    cur->players[0] = malloc(sizeof(Player));
    if (cur->players[0] == NULL) {
        perror("Error when allocating memory for player");
        return -1;
    }
    strcpy(cur->players[0]->id, player);
    cur->players[0]->socket = socket;
    cur->players[0]->port = port;
    cur->player_count = 1;

    // We finished modifying values
    pthread_mutex_unlock(&game_mutex);
    puts("Game created");
    return id;
}

// Makes the player join a game, returns player's index in the game
int join_game(int id, int socket, int port, char* player) {
    printf("Player %s is joining game %d\n", player, id);
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
            cur->players[i] = malloc(sizeof(Player));
            if (cur->players[i] == NULL) {
                return -1;
            }
            player_id = i;
            break;
        }
    }
    if (player_id == -1) {
        puts("Could not find free slot in game");
        return -1;
    }
    // Increment current game's player count
    cur->player_count++;

    // Fill in the player's info (id, socket, port)
    strcpy(cur->players[player_id]->id, player);
    cur->players[player_id]->socket = socket;
    cur->players[player_id]->port = port;

    // We finished checking / modifying values
    pthread_mutex_unlock(&game_mutex);
    return player_id;
}

int leave_game(int id) {
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
    if (cur->player_count <= 0) {
        puts("Game is empty");
        return -1;
    }

    // Remove the player from the game
    int player_id = -1;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (cur->players[i] == id) {
            player_id = i;
            break;
        }
    }
    if (player_id == -1) {
        puts("Could not find player in game");
        return -1;
    }
    // Decrement current game's player count
    cur->player_count--;

    // Free the player's info
    free(cur->players[player_id]);
    cur->players[player_id] = NULL;

    // We finished checking / modifying values
    pthread_mutex_unlock(&game_mutex);
    print_games();
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

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (games[id]->players[i] != NULL) {
            free(games[id]->players[i]);
        }
    }

    num_games--;
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
    snprintf(num_games_str, 11, "GAMES %hhu***", num_games_int);
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
            snprintf(game_str, 13, "OGAME %hhu %hhu***", game_id, player_count);
            if (safe_send(sockfd, game_str, 12) < 0) {
                puts("Error sending game");
                return -1;
            }
        }
    }
    return 0;
}

void print_games() {
    for (int i = 0; i < MAX_GAMES; i++) {
        if (game_status[i] == 1) {
            printf("-----------------\n");
            printf("Game %d\n", games[i]->id);
            for (int j = 0; j < MAX_PLAYERS; j++) {
                if (games[i]->players[j] != NULL) {
                    printf("Player : %s\n", games[i]->players[j]->id);
                }
            }
            printf("-----------------\n\n");
        }
    }
}
