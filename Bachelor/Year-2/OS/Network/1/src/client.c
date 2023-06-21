#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <unistd.h>
#include "config.h"
#include "udp_utils.h"
#include "../utils.h"

int open_server_connection(int* server_socket, struct sockaddr_in* server_sockaddr) {
    *server_socket = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);

    if (*server_socket == -1) {
        perror("Socket creation failed");
        return ERROR_CODE;
    }

    server_sockaddr->sin_family = AF_INET;
    server_sockaddr->sin_addr.s_addr = inet_addr(LOCAL_HOST);
    server_sockaddr->sin_port = htons(PORT);

    return SUCCESS_CODE;
}

int main() {
    int server_socket;
    struct sockaddr_in server_sockaddr;

    if (open_server_connection(&server_socket, &server_sockaddr) != SUCCESS_CODE) {
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

    return ERROR_CODE;
}
