#include <stdio.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>
#include "config.h"
#include "utils.h"

int open_server_connection(int* server_socket, struct sockaddr_un* server_sockaddr) {
    *server_socket = socket(AF_UNIX, SOCK_DGRAM, DEFAULT_PROTOCOL);

    if (*server_socket == -1) {
        perror("Socket creation failed");
        return ERROR_CODE;
    }

    server_sockaddr->sun_family = AF_UNIX;
    strcpy(server_sockaddr->sun_path, SERVER_SOCKET_FILE_NAME);

    return SUCCESS_CODE;
}

int main() {
    int server_socket;
    struct sockaddr_un server_sockaddr;
    if (open_server_connection(&server_socket, &server_sockaddr) != SUCCESS_CODE) {
        return ERROR_CODE;
    }

    struct sockaddr_un client_sockaddr;
    client_sockaddr.sun_family = AF_UNIX;
    sprintf(client_sockaddr.sun_path, "client_socket_%d", getpid());

    if (bind(server_socket, (const struct sockaddr*) &client_sockaddr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind() failed");
        if (close(server_socket) == -1) {
            perror("close() failed");
        }
        return ERROR_CODE;
    }

    char message_to_send[MAX_MESSAGE_LENGTH + 1] = {'\0'};
    char message_to_recv[MAX_MESSAGE_LENGTH + 1] = {'\0'};

    while (true) {
        enter_message(message_to_send);

        ssize_t sent_count;
        if (send_message(server_socket, &server_sockaddr, message_to_send, &sent_count) != SUCCESS_CODE) {
            break;
        }

        on_message_sent(&server_sockaddr, message_to_send);

        if (recv_message(server_socket, &server_sockaddr, sent_count,message_to_recv) != SUCCESS_CODE) {
            break;
        }

        on_message_recv(&server_sockaddr, message_to_recv);
    }

    if (close(server_socket) == -1) {
        perror("close() failed");
    }

    if (unlink(client_sockaddr.sun_path) == -1) {
        perror("unlink() failed");
    }

    return ERROR_CODE;
}
