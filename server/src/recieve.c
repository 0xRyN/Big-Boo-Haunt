#include "recieve.h"

// Safe receive : receive a message from a socket until last 3 bytes are ***
// Return the size of the message
int safe_receive(int sockfd, char *buffer, int buffer_size) {
    int offset = 0;
    while (1) {
        int bytes_read = recv(sockfd, buffer + offset, buffer_size - offset, 0);
        if (bytes_read < 0) {
            perror("recv");
            return -1;
        }
        offset += bytes_read;
        if (strncmp(buffer + offset - 3, "***", 3) == 0) {
            break;
        }
    }
    return offset;
}
