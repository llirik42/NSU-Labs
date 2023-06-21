#pragma once

#include <arpa/inet.h>

int send_message(int socket, const struct sockaddr_in* sockaddr, const char* message, ssize_t* sent_count);

int recv_message(int socket, struct sockaddr_in* sockaddr, size_t message_len, char* message);
