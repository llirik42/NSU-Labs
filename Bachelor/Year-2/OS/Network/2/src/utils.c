#include <stdio.h>
#include "utils.h"

void enter_message(char* message) {
    printf("Enter your message: ");
    scanf("%s", message);
}

void print_event(const char* event, const struct sockaddr_in* sockaddr, const char* content) {
    printf("%-25s%s:%d", event, inet_ntoa(sockaddr->sin_addr), ntohs(sockaddr->sin_port));

    if (content != NULL) {
        printf(". Content: %s", content);
    }

    printf("\n");
}

void on_message_sent(const struct sockaddr_in* to, const char* message) {
    print_event("Sent to", to, message);
}

void on_message_recv(const struct sockaddr_in* from, const char* message) {
    print_event("Recv from", from, message);
}

void on_waiting_on_connections() {
    printf("Waiting on connections ...\n");
}

void on_connection_lost(const struct sockaddr_in* sockaddr) {
    print_event("Connection is lost with", sockaddr, NULL);
}
