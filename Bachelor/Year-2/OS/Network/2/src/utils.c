#include <stdio.h>
#include <string.h>
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
