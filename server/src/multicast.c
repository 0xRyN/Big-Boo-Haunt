#include "multicast.h"

#include "recieve.h"

int multicast_send(char *ip, char *port, char *message) {
    // Send udp packet to ip:port
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(atoi(port));
    int res = sendto(sockfd, message, strlen(message), 0,
                     (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (res < 0) {
        perror("sendto");
        return -1;
    }
    close(sockfd);
    return 0;
}