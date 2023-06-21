#include <stdio.h>
#include <string.h>
#include "udp_utils.h"
#include "utils.h"

int send_message(int socket, const struct sockaddr_in* sockaddr, const char* message, ssize_t* sent_count) {
    const ssize_t sent = sendto(
            socket,
            message,
            strlen(message),
            DEFAULT_FLAGS,
            (const struct sockaddr*) sockaddr,
            sizeof(*sockaddr)
    );

    if (sent == -1) {
        perror("sendto() failed");
        return ERROR_CODE;
    }

    if (sent_count != NULL) {
        *sent_count = sent;
    }

    return SUCCESS_CODE;
}

int recv_message(int socket, struct sockaddr_in* sockaddr, size_t message_len, char* message) {
    socklen_t sockaddr_len;

    const ssize_t recv_count = recvfrom(
            socket,
            message,
            message_len,
            DEFAULT_FLAGS,
            (struct sockaddr*) sockaddr,
            &sockaddr_len
    );

    if (recv_count == -1) {
        perror("recv() failed");
        return ERROR_CODE;
    }

    message[recv_count] = '\0';

    return SUCCESS_CODE;
}
