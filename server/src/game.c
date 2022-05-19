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
// RETURNS STRUCT WITH GAMEID = -1 IF FAILED
PlayerInfo create_game(char* player, int socket, int port) {
    // Get the smallest id for the new game
    int id = get_game_id();
    printf("Creating game with id %d\n", id);

    // We are changing values, therefore we need to lock the mutex
    pthread_mutex_lock(&game_mutex);
    // Increase current game counter
    num_games++;

    // Fill in the Game struct
    games[id] = malloc(sizeof(Game));
    Game* cur = games[id];
    if (cur == NULL) {
        return (PlayerInfo){.player_id = -1, .game_id = -1};
    }
    cur->id = id;
    cur->players[0] = malloc(sizeof(Player));
    if (cur->players[0] == NULL) {
        perror("Error when allocating memory for player");
        return (PlayerInfo){.player_id = -1, .game_id = -1};
    }
    strcpy(cur->players[0]->id, player);
    cur->players[0]->socket = socket;
    cur->players[0]->port = port;
    cur->player_count = 1;

    // We finished modifying values
    pthread_mutex_unlock(&game_mutex);
    print_games();
    PlayerInfo info = {.player_id = 0, .game_id = id};
    return info;
}

// Makes the player join a game, returns player's index in the game
// RETURNS STRUCT WITH GAMEID = -1 IF FAILED
PlayerInfo join_game(int id, int socket, int port, char* player) { 
    printf("Player %s is joining game %d\n", player, id);
    // Check if the game exists
    if (id < 0 || id >= MAX_GAMES) {
        puts("Game does not exist (ID out of bounds)");
        return (PlayerInfo){.player_id = -1, .game_id = -1};
    }
    // We are checking different values of the struct, therefore we need to lock
    // the mutex
    pthread_mutex_lock(&game_mutex);
    Game* cur = games[id];
    if (cur == NULL) {
        puts("Game does not exist (ID not allocated)");
        return (PlayerInfo){.player_id = -1, .game_id = -1};
    }

    // Check if the game is full
    if (cur->player_count >= MAX_PLAYERS) {
        puts("Game is full");
        return (PlayerInfo){.player_id = -1, .game_id = -1};
    }

    // Add the player to the game
    int player_id = -1;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (cur->players[i] == NULL) {
            cur->players[i] = malloc(sizeof(Player));
            if (cur->players[i] == NULL) {
                return (PlayerInfo){.player_id = -1, .game_id = -1};
            }
            player_id = i;
            break;
        }
    }
    if (player_id == -1) {
        puts("Could not find free slot in game");
        return (PlayerInfo){.player_id = -1, .game_id = -1};
    }
    // Increment current game's player count
    cur->player_count++;

    // Fill in the player's info (id, socket, port)
    strcpy(cur->players[player_id]->id, player);
    cur->players[player_id]->socket = socket;
    cur->players[player_id]->port = port;

    // We finished checking / modifying values
    pthread_mutex_unlock(&game_mutex);
    print_games();
    PlayerInfo info = {.player_id = player_id, .game_id = id};
    return info;
}

int leave_game(PlayerInfo info){
    // Check if the game exists
    if (info.game_id < 0 || info.game_id >= MAX_GAMES) {
        printf("%d\n", info.game_id);
        puts("Game does not exist (ID out of bounds)");
        return -1;
    }
    // We are checking different values of the struct, therefore we need to lock
    // the mutex
    pthread_mutex_lock(&game_mutex);
    Game* cur = games[info.game_id];
    if (cur == NULL) {
        puts("Game does not exist (ID not allocated)");
        return -1;
    }
    
    // Check if the player exists
    if (info.player_id < 0 || info.player_id >= MAX_PLAYERS) {
        puts("Player does not exist (ID out of bounds)");
        return -1;
    }
    if (cur->players[info.player_id] == NULL) {
        puts("Player does not exist (ID not allocated)");
        return -1;
    }

    // Remove the player from the game
    free(cur->players[info.player_id]);
    cur->players[info.player_id] = NULL;
    cur->player_count--;
    
    // We finished checking / modifying values
    pthread_mutex_unlock(&game_mutex);
    
    if(cur->player_count == 0){
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

int nbPlayers(int id){
    return games[id]->player_count;
}

int send_game(int sockfd ,char* buffer) {
    struct LISTQ list;
    list = parse_listq(buffer);
    int game_id = list.game_id;
    if (game_status[game_id] == 1) {
        //Print game_id amount of players
        char game_str[13];
        uint8_t game_id = games[game_id]->id;
        uint8_t player_count = games[game_id]->player_count;
        snprintf(game_str, 13, "LIST! %hhu %hhu***", game_id, player_count);
        if (safe_send(sockfd, game_str, 12) < 0) {
            puts("Error sending game");
            return -1;
        }
        for(int i = 0; i < MAX_PLAYERS; i++){
            // CHECK IF THE GAME HAVE PLAYERS IN IT
            if(games[game_id]->players[i] != NULL){ // TODO VOIR PQ FAUT BOUCLER PR N MESSAGE ET PAS DANS SENDGAMES
                //printf("PLAYR %s***\n",games[game_id]->players[i]->id);
                char res_buffer[18];
                snprintf(res_buffer, 18, "PLAYR %s***", games[game_id]->players[i]->id);
                if (safe_send(sockfd, res_buffer, 17) < 0) {
                    puts("Error sending game");
                    return -1;
                }
            }
        }
          
    }else{
        char game_str[9];
        snprintf(game_str, 9, "DUNNO***");
        if (safe_send(sockfd, game_str, 9) < 0) {
                puts("Error sending game");
                return -1;
        }  
        return -1;
    }
    return 0;
}

void print_games() {
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    for (int i = 0; i < MAX_GAMES; i++) {
        if (game_status[i] == 1) {
            printf("-----------------\n");
            printf("Game %d, player count : %d\n", games[i]->id,
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
