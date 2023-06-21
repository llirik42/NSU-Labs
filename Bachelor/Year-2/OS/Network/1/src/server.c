#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <unistd.h>
#include "config.h"
#include "utils.h"

int create_server_socket(int* server_socket) {
    *server_socket = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);

    if (*server_socket == -1) {
        perror("Socket creation failed");
        return ERROR_CODE;
    }

    const struct sockaddr_in server_sockaddr = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = htonl(INADDR_ANY),
            .sin_port = htons(PORT)
    };

    if (bind(*server_socket, (const struct sockaddr *) &server_sockaddr, sizeof(server_sockaddr)) == -1) {
        perror("bind() failed");
        if (close(*server_socket) == -1) {
            perror("close() failed");
        }
        return ERROR_CODE;
    }

    return SUCCESS_CODE;
}

int main() {
    int server_socket;

    if (create_server_socket(&server_socket) != SUCCESS_CODE) {
        return ERROR_CODE;
    }

    char message[MAX_MESSAGE_LENGTH] = {'\0'};

    on_waiting_on_connections();

    while (true) {
        struct sockaddr_in client_sockaddr;
        if (recv_message(server_socket, &client_sockaddr, MAX_MESSAGE_LENGTH, message) != SUCCESS_CODE) {
            break;
        }

        on_message_recv(&client_sockaddr, message);

        if (send_message(server_socket, &client_sockaddr, message, NULL) != SUCCESS_CODE) {
            break;
        }

        on_message_sent(&client_sockaddr, message);
    }

    if (close(server_socket) == -1) {
        perror("close() failed");
    }

    return ERROR_CODE;
}
