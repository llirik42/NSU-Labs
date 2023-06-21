#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "tcp_utils.h"
#include "utils.h"

int recv_flags(int socket, size_t message_len, char* message, int flags) {
    const ssize_t recv_count = recv(socket, message, message_len, flags);

    if (recv_count == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return DATA_NOT_AVAILABLE;
        }
        perror("recv() failed");
        return ERROR_CODE;
    }

    if (recv_count == 0) {
        return ERROR_CODE;
    }

    message[recv_count] = '\0';

    return SUCCESS_CODE;
}

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
    return recv_flags(socket, message_len, message, DEFAULT_FLAGS);
}

int recv_message_non_blocking(int socket, size_t message_len, char* message) {
    return recv_flags(socket, message_len, message, MSG_DONTWAIT);
}
