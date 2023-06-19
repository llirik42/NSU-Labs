#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include "config.h"

#define BACKLOG 5
#define ANY_CHILD_PROCESS_PID (-1)

void handle_connection(int client_socket_fd, const struct sockaddr_in* client_sockaddr) {
    char message[MAX_MESSAGE_LENGTH] = {'\0'};

    while (true) {
        const ssize_t recv_count = recv(client_socket_fd, message, MAX_MESSAGE_LENGTH, 0);

        message[recv_count] = '\0';

        if (recv_count == -1) {
            perror("recv() failed");
            return;
        }

        if (recv_count == 0) {
            printf("Connection with %s:%d is lost\n",
                   inet_ntoa(client_sockaddr->sin_addr),
                   ntohs(client_sockaddr->sin_port));
            return;
        }

        printf("Server received from %s:%d. Message: %s\n",
               inet_ntoa(client_sockaddr->sin_addr),
               ntohs(client_sockaddr->sin_port),
               message);

        const ssize_t send_count = send(client_socket_fd, message, recv_count, 0);

        if (send_count == -1) {
            perror("sendto() failed");
            return;
        }

        printf("Server send: %s\n", message);
    }
}

int wait_for_all_children_async() {
    while (true) {
        int wstatus;
        int return_code = waitpid(ANY_CHILD_PROCESS_PID, &wstatus, WNOHANG);

        if (return_code <= 0) {
            return return_code;
        }
    }
}

int wait_for_all_children_sync() {
    while (true) {
        int wstatus;
        int return_code = waitpid(ANY_CHILD_PROCESS_PID, &wstatus, 0);

        if (return_code <= 0) {
            return return_code;
        }
    }
}

int main() {
    const int sever_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (sever_socket == -1) {
        perror("Socket creation failed");
        return ERROR_CODE;
    }

    const struct sockaddr_in server_sockaddr = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = htonl(INADDR_ANY),
            .sin_port = htons(PORT)
    };

    if (bind(sever_socket, (const struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr)) == -1) {
        perror("bind() failed");
        if (close(sever_socket) == -1) {
            perror("close() failed");
        }
        return ERROR_CODE;
    }

    if (listen(sever_socket, BACKLOG) == -1) {
        perror("listen() failed");
        if (close(sever_socket) == -1) {
            perror("close() failed");
        }
        return ERROR_CODE;
    }

    struct sockaddr_in client_sockaddr;
    socklen_t socklen = sizeof(client_sockaddr);
    int client_socket_fd;
    printf("Waiting on connections ...\n");
    while (true) {
        client_socket_fd = accept(sever_socket, (struct sockaddr*) &client_sockaddr, &socklen);

        const int pid = fork();

        if (pid < 0) {
            perror("fork() failed");
            break;
        }
        if (pid > 0) {
            // Parent

            close(client_socket_fd);
            if (wait_for_all_children_async() == -1) {
                perror("waitpid() failed");
                break;
            }

            continue;
        }

        // Child work
        handle_connection(client_socket_fd, &client_sockaddr);
        close(client_socket_fd);
        close(sever_socket);
        return ERROR_CODE;
    }

    if (close(sever_socket) == -1) {
        perror("close() failed");
    }

    if (wait_for_all_children_sync() == -1) {
        perror("waiting for children after shutting down failed");
    }

    return ERROR_CODE;
}
