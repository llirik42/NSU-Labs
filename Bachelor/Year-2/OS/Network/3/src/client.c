#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "config.h"

void print_enter_message() {
    printf("Enter your message: ");
}

int main() {
    const pid_t pid = getpid();

    const int sock = socket(AF_INET, SOCK_STREAM, 0);

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

    if (connect(sock, (const struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr))) {
        perror("connect() failed");
        return ERROR_CODE;
    }

    bool error = false;

    while (true) {
        print_enter_message();
        if (scanf("%s", send_message) < 0) {
            break;
        }

        const ssize_t send_count = send(sock, send_message, strlen(send_message), 0);

        if (send_count == -1) {
            perror("sendto() failed");
            error = true;
            break;
        }

        printf("%d send: %s\n", pid, send_message);

        const ssize_t recv_count = recv(sock, recv_message, send_count, 0);

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
