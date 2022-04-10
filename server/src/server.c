#include "server.h"

int init_server() {
    // Create the socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

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
    int sockfd = *(int *)arg;
    int interact_return = interact(sockfd);
    if (interact_return < 0) {
        puts("Error interacting with client");
        perror("interact");
        return NULL;
    }
}

int main() {
    // Init server
    int sockfd = init_server();
    if (sockfd < 0) {
        return -1;
    }

    // Create a new thread for each client
    int i = 0;
    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("accept");
            return -1;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, &newsockfd);
    }
    print_games();
    return 0;

error:
    perror("Fatal error ! Exiting...");
    return -1;
}