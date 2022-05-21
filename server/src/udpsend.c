#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int send_udp(char *ip, char *port, char *message, char *id_send) {
    char porto[4 + 1];
    memcpy(&porto, port, 4);
    porto[4] = '\0';
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
    server_addr.sin_port = htons(atoi(porto));
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Send the message to server:
    char res_buffer[strlen(message) + 18];
    sprintf(res_buffer, "MESSP %s %s+++", id_send, message);
    if (sendto(socket_desc, res_buffer, strlen(message) + 18, 0,
               (struct sockaddr *)&server_addr, server_struct_length) < 0) {
        printf("Unable to send message\n");
        return -1;
    }
    free(message);
    free(ip);
    free(port);
    // Close the socket:
    close(socket_desc);

    return 0;
}