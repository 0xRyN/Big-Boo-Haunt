#include "server.h"

int init_server() {
    // Create the socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    int option = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option,
               sizeof(option));  // TO REMOVE

    // Bind the socket to the port
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(PORT));

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        return -1;
    }

    // Listen for incoming connections
    if (listen(sockfd, 0) < 0) {
        perror("listen");
        return -1;
    }

    return sockfd;
}

void *handle_client(void *arg) {
    Thread_Args *args = (Thread_Args *)arg;
    int interact_return = interact(args);
    if (interact_return < 0) {
        puts("Error interacting with client");
        perror("interact");
    }
    close(args->sockfd);
    free(args);
    // TODO : Join threads at the end and check for errors
    return NULL;
}

int main() {
    // Init server
    int sockfd = init_server();
    if (sockfd < 0) {
        return -1;
    }

    int init_return = init_games();
    if (init_return < 0) {
        puts("Error initializing games");
        return -1;
    }

    // Create a new thread for each client
    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("accept");
            goto error;
            return -1;
        }

        Thread_Args *args = malloc(sizeof(Thread_Args));
        args->sockfd = newsockfd;
        args->client_addr = cli_addr;

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, args);
    }
    print_games();
    return 0;

error:
    perror("Fatal error ! Exiting...");
    return -1;
}
