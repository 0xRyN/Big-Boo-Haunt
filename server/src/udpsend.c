#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int send_udp(char *ip, char *port, char *message) {
    char porto[4];
    memcpy(&porto, port, 4);
    printf("Sending udp packet to %s:%s\n", ip, porto);
    printf("message: %s\n", message);
    int socket_desc;
    struct sockaddr_in server_addr;
    int server_struct_length = sizeof(server_addr);

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (socket_desc < 0) {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8089);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Send the message to server:
    printf("ICI\n");
    if (sendto(socket_desc, message, strlen(message), 0,
               (struct sockaddr *)&server_addr, server_struct_length) < 0) {
        printf("Unable to send message\n");
        return -1;
    }
    printf("DANY\n");

    // Close the socket:
    close(socket_desc);

    return 0;
}