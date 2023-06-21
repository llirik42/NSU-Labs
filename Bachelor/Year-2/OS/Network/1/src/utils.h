#pragma once

#include <arpa/inet.h>

#define DEFAULT_PROTOCOL 0
#define DEFAULT_FLAGS 0

#define SUCCESS_CODE 0
#define ERROR_CODE 1
#define DATA_NOT_AVAILABLE 2

void enter_message(char* message);

void print_event(const char* event, const struct sockaddr_in* sockaddr, const char* content);

void on_message_sent(const struct sockaddr_in* to, const char* message);

void on_message_recv(const struct sockaddr_in* from, const char* message);

void on_waiting_on_connections();

void on_connection_lost(const struct sockaddr_in* sockaddr);
