#pragma once

#include <arpa/inet.h>

struct client_t {
    int socket_fd;
    struct sockaddr_in sockaddr;
};

int send_message(int socket, const char* message, ssize_t* sent_count);

int recv_message(int socket, size_t message_len, char* message);
