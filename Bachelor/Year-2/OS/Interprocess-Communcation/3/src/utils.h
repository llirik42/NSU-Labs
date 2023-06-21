#pragma once

#include <sys/un.h>

#define DEFAULT_PROTOCOL 0
#define DEFAULT_FLAGS 0

#define SUCCESS_CODE 0
#define ERROR_CODE 1

void enter_message(char* message);

void print_event(const char* event, const struct sockaddr_un* sockaddr, const char* content);

void on_message_sent(const struct sockaddr_un* to, const char* message);

void on_message_recv(const struct sockaddr_un* from, const char* message);

void on_waiting_on_connections();

int send_message(int socket, const struct sockaddr_un* sockaddr, const char* message, ssize_t* sent_count);

int recv_message(int socket, struct sockaddr_un* sockaddr, size_t message_len, char* message);
