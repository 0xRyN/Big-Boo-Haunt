#include "send.h"

// Safe send : send a message to a socket, send exactly buffer_size bytes
// Return the size of the message sent
int safe_send(int sockfd, char *buffer, int buffer_size) {
    int offset = 0;
    while (1) {
        int bytes_sent = send(sockfd, buffer + offset, buffer_size - offset, 0);
        if (bytes_sent < 0) {
            perror("send");
            return -1;
        }
        offset += bytes_sent;
        if (offset == buffer_size) {
            break;
        }
    }
    return offset;
}
