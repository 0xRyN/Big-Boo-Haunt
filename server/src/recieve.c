#include "recieve.h"

// Safe receive : receive a message from a socket until last 3 bytes are ***
// Return the size of the message
int safe_receive(int sockfd, char *buffer, int buffer_size) {
    int offset = 0;
    while (1) {
        int bytes_read = recv(sockfd, buffer + offset, buffer_size - offset, 0);
        printf("Bytes read : %d\n", bytes_read);
        printf("Buffer is: %.*s\n", bytes_read, buffer);
        if (bytes_read < 0) {
            perror("recv");
            return -1;
        }
        if (bytes_read == 0) {
            printf("Buffer is : %.*s\n", 10, buffer);
            puts("Connection closed !");
            perror("Read 0 bytes");
            return -1;
        }
        offset += bytes_read;
        if (memcmp(buffer + offset - 3 - 1, "***", 3) == 0) {
            puts("We received the end of the message !");
            return offset;
        }
    }
    return -1;
}
