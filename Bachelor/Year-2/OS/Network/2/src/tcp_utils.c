#include <stdio.h>
#include <string.h>
#include "tcp_utils.h"
#include "../utils.h"

int send_message(int socket, const char* message, ssize_t* sent_count) {
    const ssize_t sent = send(socket, message, strlen(message), DEFAULT_FLAGS);

    if (sent == -1) {
        perror("sendto() failed");
        return ERROR_CODE;
    }

    if (sent_count != NULL) {
        *sent_count = sent;
    }

    return SUCCESS_CODE;
}

int recv_message(int socket, size_t message_len, char* message) {
    const ssize_t recv_count = recv(socket, message, message_len, DEFAULT_FLAGS);

    if (recv_count == -1) {
        perror("recv() failed");
        return ERROR_CODE;
    }

    if (recv_count == 0) {
        return ERROR_CODE;
    }

    message[recv_count] = '\0';

    return SUCCESS_CODE;
}
