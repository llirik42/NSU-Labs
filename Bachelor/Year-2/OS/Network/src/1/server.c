#include <stdio.h>
#include "config.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

int main() {
    const int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == -1) {
        perror("Socket creation failed");
        return ERROR_CODE;
    }

    struct sockaddr_in server_sockaddr;
    memset(&server_sockaddr, 0, sizeof(server_sockaddr));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_sockaddr.sin_port = htons(PORT);

    if (bind(sock, (const struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr)) == -1) {
        perror("bind() failed");
        if (close(sock) == -1) {
            perror("close() failed");
        }
        return ERROR_CODE;
    }

    char message[MAX_MESSAGE_LENGTH] = {'\0'};

    struct sockaddr_in client_sockaddr;
    socklen_t socklen = sizeof(client_sockaddr);
    printf("Waiting on connections ...\n");
    while (true) {
        ssize_t recv_count = recvfrom(sock,
                                  message,
                                  MAX_MESSAGE_LENGTH,
                                  0,
                                  (struct sockaddr*) &client_sockaddr,
                                  &socklen);

        message[recv_count] = '\0';

        if (recv_count == -1) {
            perror("recv() failed");
            break;
        }

        printf("Server received from %s:%d. Message: %s\n",
               inet_ntoa(client_sockaddr.sin_addr),
               ntohs(client_sockaddr.sin_port),
               message);

        ssize_t send_count = sendto(sock,
                                    message,
                                    recv_count,
                                    0,
                                    (const struct sockaddr*) &client_sockaddr,
                                            socklen);

        if (send_count == -1) {
            perror("sendto() failed");
            break;
        }

        printf("Server send: %s\n", message);
    }

    if (close(sock) == -1) {
        perror("close() failed");
    }

    return ERROR_CODE;
}
