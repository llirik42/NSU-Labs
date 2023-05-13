#include <stdio.h>
#include "config.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

void print_enter_message() {
    printf("Enter your message: ");
}

int main() {
    const pid_t pid = getpid();

    const int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == -1) {
        perror("Socket creation failed");
        return ERROR_CODE;
    }

    const struct sockaddr_in server_sockaddr = {
            .sin_family = AF_INET,
            .sin_addr = inet_addr(LOCAL_HOST),
            .sin_port = htons(PORT),
    };

    char send_message[MAX_MESSAGE_LENGTH] = {'\0'};
    char recv_message[MAX_MESSAGE_LENGTH] = {'\0'};

    bool error = false;
    socklen_t socklen = sizeof(server_sockaddr);

    while (true) {
        print_enter_message();
        if (scanf("%s", send_message) < 0) {
            break;
        }

        ssize_t send_count = sendto(sock,
                                    send_message,
                                    strlen(send_message),
                                    0,
                                    (const struct sockaddr*) &server_sockaddr,
                                            sizeof(server_sockaddr));

        if (send_count == -1) {
            perror("sendto() failed");
            error = true;
            break;
        }

        printf("%d send: %s\n", pid, send_message);

        ssize_t recv_count = recvfrom(sock,
                                      recv_message,
                                      send_count,
                                      0,
                                      (struct sockaddr *) &server_sockaddr,
                                      &socklen);

        recv_message[recv_count] = '\0';

        if (recv_count == -1) {
            perror("recv() failed");
            error = true;
            break;
        }

        printf("%d received from %s:%d. Message: %s\n",
               pid,
               inet_ntoa(server_sockaddr.sin_addr),
               ntohs(server_sockaddr.sin_port),
               recv_message);
    }

    if (close(sock) == -1) {
        perror("close() failed");
        error = true;
    }

    return error ? ERROR_CODE : SUCCESS_CODE;
}
