#include "game.h"

int num_games = 0;
int num_gameplayable = 0;
int game_status[MAX_GAMES];
Game *games[MAX_GAMES];
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
// RETURNS STRUCT WITH GAMEID = -1 IF FAILED
PlayerInfo create_game(char *player, int socket, char *port,
                       struct sockaddr_in *addr) {
    // Get the smallest id for the new game
    int id = get_game_id();
    printf("Creating game with id %d\n", id);

    // We are changing values, therefore we need to lock the mutex
    pthread_mutex_lock(&game_mutex);
    // Increase current game counter
    num_games++;
    num_gameplayable++;

    // Fill in the Game struct
    games[id] = malloc(sizeof(Game));
    Game *cur = games[id];
    if (cur == NULL) {
        pthread_mutex_unlock(&game_mutex);
        return (PlayerInfo){.player_id = -1, .game_id = -1};
    }

    // Fill in the Player struct
    for (int i = 0; i < MAX_PLAYERS; i++) {
        cur->players[i] = NULL;
    }

    cur->id = id;
    cur->players[0] = malloc(sizeof(Player));
    cur->mazeID = 1;

    // TODO : Remove this from here.
    // Read file called 1 in Maze folder and print the amount of lines
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int size = 0;
    fp = fopen("Maze/1.txt", "r");  // TODO : Au lieu de 1 mettre game_id
    if (fp == NULL) exit(EXIT_FAILURE);
    int width = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        width = read;
        size++;
    }
    fclose(fp);
    if (line) free(line);
    cur->mazeWidth = width;
    cur->mazeHeight = size;

    if (cur->players[0] == NULL) {
        perror("Error when allocating memory for player");
        pthread_mutex_unlock(&game_mutex);
        return (PlayerInfo){.player_id = -1, .game_id = -1};
    }
    strcpy(cur->players[0]->id, player);
    cur->players[0]->socket = socket;
    memcpy(cur->players[0]->port, port, 4);
    cur->players[0]->port[4] = '\0';
    cur->players[0]->addr = addr;
    cur->players[0]->score = 0;
    cur->player_count = 1;
    cur->amout_of_ready_players = 0;

    // We finished modifying values
    pthread_mutex_unlock(&game_mutex);
    print_games();
    PlayerInfo info = {.player_id = 0, .game_id = id};
    return info;
}

// Makes the player join a game, returns player's index in the game
// RETURNS STRUCT WITH GAMEID = -1 IF FAILED
PlayerInfo join_game(int id, int socket, char *port, char *player,
                     struct sockaddr_in *addr) {
    printf("Player %s is joining game %d\n", player, id);
    // Check if the game exists
    if (id < 0 || id >= MAX_GAMES) {
        puts("Game does not exist (ID out of bounds)");
        return (PlayerInfo){.player_id = -1, .game_id = -1};
    }
    // We are checking different values of the struct, therefore we need to lock
    // the mutex
    pthread_mutex_lock(&game_mutex);
    Game *cur = games[id];
    if (cur == NULL) {
        puts("Game does not exist (ID not allocated)");
        pthread_mutex_unlock(&game_mutex);
        return (PlayerInfo){.player_id = -1, .game_id = -1};
    }

    // Check if the game is full
    if (cur->player_count >= MAX_PLAYERS) {
        puts("Game is full");
        pthread_mutex_unlock(&game_mutex);
        return (PlayerInfo){.player_id = -1, .game_id = -1};
    }

    // Check if there isn't a player with the same name
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (cur->players[i] != NULL &&
            strcmp(cur->players[i]->id, player) == 0) {
            puts("Player already exists");
            pthread_mutex_unlock(&game_mutex);
            return (PlayerInfo){.player_id = -1, .game_id = -1};
        }
    }

    // Add the player to the game
    int player_id = -1;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (cur->players[i] == NULL) {
            cur->players[i] = malloc(sizeof(Player));
            if (cur->players[i] == NULL) {
                pthread_mutex_unlock(&game_mutex);
                return (PlayerInfo){.player_id = -1, .game_id = -1};
            }
            player_id = i;
            break;
        }
    }
    if (player_id == -1) {
        puts("Could not find free slot in game");
        pthread_mutex_unlock(&game_mutex);
        return (PlayerInfo){.player_id = -1, .game_id = -1};
    }
    // Increment current game's player count
    cur->player_count++;

    // Fill in the player's info (id, socket, port)
    strcpy(cur->players[player_id]->id, player);
    cur->players[player_id]->socket = socket;
    memcpy(cur->players[player_id]->port, port, 4);
    cur->players[player_id]->port[4] = '\0';
    cur->players[player_id]->addr = addr;
    cur->players[player_id]->score = 0;

    // We finished checking / modifying values
    pthread_mutex_unlock(&game_mutex);
    print_games();
    PlayerInfo info = {.player_id = player_id, .game_id = id};
    return info;
}

int leave_game(PlayerInfo info) {
    // Check if the game exists
    if (info.game_id < 0 || info.game_id >= MAX_GAMES) {
        printf("%d\n", info.game_id);
        puts("Game does not exist (ID out of bounds)");
        return -1;
    }
    // We are checking different values of the struct, therefore we need to lock
    // the mutex
    pthread_mutex_lock(&game_mutex);
    Game *cur = games[info.game_id];
    if (cur == NULL) {
        puts("Game does not exist (ID not allocated)");
        pthread_mutex_unlock(&game_mutex);
        return -1;
    }

    // Check if the player exists
    if (info.player_id < 0 || info.player_id >= MAX_PLAYERS) {
        puts("Player does not exist (ID out of bounds)");
        pthread_mutex_unlock(&game_mutex);
        return -1;
    }
    if (cur->players[info.player_id] == NULL) {
        puts("Player does not exist (ID not allocated)");
        pthread_mutex_unlock(&game_mutex);
        return -1;
    }

    // check if the player was ready
    if (cur->players[info.player_id]->is_ready == 1) {
        cur->amout_of_ready_players--;
    }
    // Remove the player from the game
    free(cur->players[info.player_id]);
    cur->players[info.player_id] = NULL;
    cur->player_count--;

    // We finished checking / modifying values
    pthread_mutex_unlock(&game_mutex);

    if (cur->player_count == 0) {
        destroy_game(info.game_id);
    }
    print_games();
    return 0;
}

int destroy_game(int id) {
    puts("hello");
    // Check if the game exists
    if (id < 0 || id >= MAX_GAMES) {
        puts("Game does not exist (ID out of bounds)");
        pthread_mutex_unlock(&game_mutex);
        return -1;
    }
    // We are changing values, therefore we need to lock the mutex
    pthread_mutex_lock(&game_mutex);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (games[id]->players[i] != NULL) {
            free(games[id]->players[i]);
        }
    }
    printf("Game %d destroyed\n", id);

    num_games--;
    if (game_status[id] == 1) {
        num_gameplayable--;
    }
    game_status[id] = -1;
    free(games[id]);

    // We finished modifying values
    pthread_mutex_unlock(&game_mutex);
    return 0;
}

// Function to send games to the client
int send_games(int sockfd) {
    // TODO: the games

    // First, send number of games
    char num_games_str[10];
    uint8_t num_games_int = num_gameplayable;
    memcpy(num_games_str, "GAMES ", 6);
    memcpy(num_games_str + 6, &num_games_int, 1);
    memcpy(num_games_str + 7, "***", 3);
    if (safe_send(sockfd, num_games_str, 10) < 0) {
        puts("Error sending number of games");
        return -1;
    }

    // Then, send each game
    for (int i = 0; i < MAX_GAMES; i++) {
        // Check if game exists (status = 1)
        if (game_status[i] == 1) {
            char game_str[12];
            uint8_t game_id = games[i]->id;
            uint8_t player_count = games[i]->player_count;
            memcpy(game_str, "OGAME ", 6);
            memcpy(game_str + 6, &game_id, 1);
            memcpy(game_str + 7, " ", 1);
            memcpy(game_str + 8, &player_count, 1);
            memcpy(game_str + 9, "***", 3);
            // snprintf(game_str, 13, "OGAME %hhu %hhu***", game_id,
            // player_count);
            if (safe_send(sockfd, game_str, 12) < 0) {
                puts("Error sending game");
                return -1;
            }
        }
    }
    return 0;
}

int increment_amout_of_ready_players(PlayerInfo info) {
    // Check if the game exists
    if (info.game_id < 0 || info.game_id >= MAX_GAMES) {
        puts("Game does not exist (ID out of bounds)");
        return -1;
    }
    // We are checking different values of the struct, therefore we need to lock
    // the mutex
    pthread_mutex_lock(&game_mutex);
    Game *cur = games[info.game_id];
    if (cur == NULL) {
        puts("Game does not exist (ID not allocated)");
        pthread_mutex_unlock(&game_mutex);
        return -1;
    }

    // Increment the amount of ready players
    cur->players[info.player_id]->is_ready = 1;
    cur->amout_of_ready_players++;

    if (cur->amout_of_ready_players == cur->player_count &&
        cur->player_count > MIN_PLAYERS) {
        game_status[info.game_id] = 2;
        num_gameplayable--;
        pthread_mutex_unlock(&game_mutex);
        return 1;
    }
    // We finished checking / modifying values
    pthread_mutex_unlock(&game_mutex);
    print_games();
    return 0;
}

int ask_size(int sockfd, char *buffer) {
    struct SIZEQ list;
    list = parse_sizeq(buffer);
    int game_id = list.game_id;
    if (game_status[game_id] == 1) {
        // Send the size of the maze
        char size_str[16];
        uint16_t height_int = games[list.game_id]->mazeHeight;
        uint16_t width_int = games[list.game_id]->mazeWidth;
        u_int8_t game_id_int = games[list.game_id]->id;
        memcpy(size_str, "SIZE! ", 6);
        memcpy(size_str + 6, &game_id_int, 1);
        memcpy(size_str + 7, " ", 1);
        memcpy(size_str + 8, &height_int, 2);
        memcpy(size_str + 10, " ", 1);
        memcpy(size_str + 11, &width_int, 2);
        memcpy(size_str + 13, "***", 3);
        if (safe_send(sockfd, size_str, 16) < 0) {
            puts("Error sending size");
            return -1;
        }
    } else {
        char game_str[9];
        snprintf(game_str, 9, "DUNNO***");
        if (safe_send(sockfd, game_str, 9) < 0) {
            puts("Error sending game");
            return -1;
        }
    }
    return 0;
}

int send_game(int sockfd, char *buffer) {
    struct LISTQ list;
    list = parse_listq(buffer);
    int game_id = list.game_id;
    if (game_status[game_id] == 1) {
        // Print game_id amount of players
        char game_str[13];
        uint8_t game_id = games[game_id]->id;
        uint8_t player_count = games[game_id]->player_count;
        memcpy(game_str, "LIST! ", 6);
        memcpy(game_str + 6, &game_id, 1);
        memcpy(game_str + 7, " ", 1);
        memcpy(game_str + 8, &player_count, 1);
        memcpy(game_str + 9, "***", 3);
        game_str[12] = '\0';
        if (safe_send(sockfd, game_str, 12) < 0) {
            puts("Error sending game");
            return -1;
        }
        for (int i = 0; i < MAX_PLAYERS; i++) {
            // CHECK IF THE GAME HAVE PLAYERS IN IT
            if (games[game_id]->players[i] !=
                NULL) {  // TODO VOIR PQ FAUT BOUCLER PR N MESSAGE ET PAS DANS
                char res_buffer[18];
                snprintf(res_buffer, 18, "PLAYR %s***",
                         games[game_id]->players[i]->id);
                if (safe_send(sockfd, res_buffer, 17) < 0) {
                    puts("Error sending game");
                    return -1;
                }
            }
        }
    } else {
        char game_str[9];
        snprintf(game_str, 9, "DUNNO***");
        if (safe_send(sockfd, game_str, 9) < 0) {
            puts("Error sending game");
            return -1;
        }
    }
    return 0;
}

Game *get_game(int id) { return games[id]; }

void print_games() {
    printf(
        "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
        "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    for (int i = 0; i < MAX_GAMES; i++) {
        if (game_status[i] == 1) {
            printf("-----------------\n");
            printf("Game %d, player count : %d areready %d/%d\n", games[i]->id,
                   games[i]->player_count, games[i]->amout_of_ready_players,
                   games[i]->player_count);
            for (int j = 0; j < MAX_PLAYERS; j++) {
                if (games[i] != NULL && games[i]->players[j] != NULL) {
                    printf("Player name : %s, id : %d\n",
                           games[i]->players[j]->id, j);
                }
            }
            printf("-----------------\n\n");
        }
    }
}
