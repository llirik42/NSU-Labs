#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include "config.h"
#include "utils.h"

#define BACKLOG 5
#define ANY_CHILD_PROCESS_PID (-1)
#define DEFAULT_WAIT_OPTIONS 0

int create_server_socket(int* server_socket) {
    *server_socket = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);

    if (*server_socket == -1) {
        perror("Socket creation failed");
        return ERROR_CODE;
    }

    const struct sockaddr_in server_sockaddr = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = htonl(INADDR_ANY),
            .sin_port = htons(PORT)
    };

    if (bind(*server_socket, (const struct sockaddr *) &server_sockaddr, sizeof(server_sockaddr)) == -1) {
        perror("bind() failed");
        if (close(*server_socket) == -1) {
            perror("close() failed");
        }
        return ERROR_CODE;
    }

    if (listen(*server_socket, BACKLOG) == -1) {
        perror("listen() failed");
        if (close(*server_socket) == -1) {
            perror("close() failed");
        }
        return ERROR_CODE;
    }

    return SUCCESS_CODE;
}

int wait_for_all_children(int options) {
    while (true) {
        int wstatus;
        int return_code = waitpid(ANY_CHILD_PROCESS_PID, &wstatus, options);

        if (return_code <= 0) {
            return return_code;
        }
    }
}

int wait_for_all_children_async() {
    return wait_for_all_children(WNOHANG);
}

int wait_for_all_children_sync() {
    return wait_for_all_children(DEFAULT_WAIT_OPTIONS);
}

void on_connection(const struct sockaddr_in* sockaddr) {
    print_event("New client", sockaddr, NULL);
}

int handle_connection(int server_socket, struct client_t* client) {
    socklen_t client_sockaddr_len = sizeof(client->sockaddr);
    client->socket_fd = accept(server_socket, (struct sockaddr*) &client->sockaddr, &client_sockaddr_len);

    if (client->socket_fd == -1) {
        perror("accept() failed");
        return ERROR_CODE;
    }

    on_connection(&client->sockaddr);

    return SUCCESS_CODE;
}

void handle_client(const struct client_t* client) {
    char message[MAX_MESSAGE_LENGTH] = {'\0'};

    while (true) {
        if (recv_message(client->socket_fd, MAX_MESSAGE_LENGTH, message) != SUCCESS_CODE) {
            on_connection_lost(&client->sockaddr);
            break;
        }

        on_message_recv(&client->sockaddr, message);

        if (send_message(client->socket_fd, message, NULL) != SUCCESS_CODE) {
            break;
        }

        on_message_sent(&client->sockaddr, message);
    }
}

int main() {
    int server_socket;

    if (create_server_socket(&server_socket) != SUCCESS_CODE) {
        return ERROR_CODE;
    }

    on_waiting_on_connections();

    while (true) {
        struct client_t client;
        handle_connection(server_socket, &client);

        const int pid = fork();

        if (pid < 0) {
            perror("fork() failed");
            break;
        }

        // Parent
        if (pid > 0) {
            close(client.socket_fd);
            if (wait_for_all_children_async() == -1) {
                perror("waitpid() failed");
                break;
            }
            continue;
        }

        // Child
        handle_client(&client);

        if (close(client.socket_fd) == -1) {
            perror("close(client_socket) failed");
        }
        if (close(server_socket) == -1) {
            perror("close(server_socket) failed");
        }

        return ERROR_CODE;
    }

    if (close(server_socket) == -1) {
        perror("close() failed");
    }

    if (wait_for_all_children_sync() == -1) {
        perror("waiting for children after shutting down failed");
    }

    return ERROR_CODE;
}
