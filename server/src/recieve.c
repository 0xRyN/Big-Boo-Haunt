#include "recieve.h"

// Safe receive : receive a message from a socket until last 3 bytes are ***
// Return the size of the message

// If returns -2, the connection is closed
int safe_receive(int sockfd, char *buffer, int buffer_size) {
    int offset = 0;
    while (1) {
        int bytes_read = recv(sockfd, buffer + offset, buffer_size - offset, 0);
        // printf("Bytes read : %d\n", bytes_read);
        // printf("Buffer is: %.*s\n", bytes_read, buffer);
        if (bytes_read < 0) {
            perror("recv");
            return -1;
        }
        if (bytes_read == 0) {
            // printf("Buffer is : %.*s\n", 10, buffer);
            puts("Connection closed !");
            return -2;
        }
        offset += bytes_read;
        // printf("Last 3 bytes in hex : %x %x %x\n", buffer[offset - 3],
        // buffer[offset - 2], buffer[offset - 1]);
        if (memcmp(buffer + offset - 3, "***", 3) == 0) {
            // puts("We received the end of the message !");
            return offset;
        }
    }
    return -1;
}
