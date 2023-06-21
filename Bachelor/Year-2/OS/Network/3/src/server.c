#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/epoll.h>
#include "config.h"
#include "tcp_utils.h"
#include "clients_container.h"
#include "../utils.h"

#define BACKLOG 5
#define ANY_CHILD_PROCESS_PID (-1)
#define DEFAULT_WAIT_OPTIONS 0
#define DEFAULT_EPOLL_FLAGS 0
#define MAX_EVENTS_COUNT 64
#define NO_TIMEOUT (-1)

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

int create_epoll(int* epoll_fd) {
    *epoll_fd = epoll_create1(DEFAULT_EPOLL_FLAGS);
    if (*epoll_fd == -1) {
        perror("epoll_creation failed");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

int add_fd_to_epoll_for_read(int fd, int epoll_fd) {
    struct epoll_event configuration_epoll_event = {
            .events = EPOLLIN,
            .data.fd = fd
    };

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &configuration_epoll_event) == -1) {
        perror("epoll_ctl(add) failed");
        return ERROR_CODE;
    }

    return SUCCESS_CODE;
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

int handle_client(const struct client_t* client) {
    char message[MAX_MESSAGE_LENGTH] = {'\0'};

    while (true) {
        const int code = recv_message_non_blocking(client->socket_fd, MAX_MESSAGE_LENGTH, message);

        if (code == ERROR_CODE) {
            on_connection_lost(&client->sockaddr);
            return ERROR_CODE;
        }
        if (code == DATA_NOT_AVAILABLE) {
            return SUCCESS_CODE;
        }

        on_message_recv(&client->sockaddr, message);

        if (send_message(client->socket_fd, message, NULL) != SUCCESS_CODE) {
            break;
        }

        on_message_sent(&client->sockaddr, message);
    }
    return SUCCESS_CODE;
}

int handle_ready_sockets(int server_socket, int epoll_fd, struct clients_container_t* clients_container,
                         const struct epoll_event* events, int count_of_ready_fd) {
    for (int i = 0; i < count_of_ready_fd; i++) {
        const struct epoll_event event = events[i];
        const int fd = event.data.fd;

        if (fd == server_socket) {
            struct client_t client;
            handle_connection(server_socket, &client);
            add_client(clients_container, client);
            if (add_fd_to_epoll_for_read(client.socket_fd, epoll_fd) != SUCCESS_CODE) {
                return ERROR_CODE;
            }
        }
        else {
            struct client_t client = get_client(clients_container, fd);
            if (handle_client(&client) != SUCCESS_CODE) {
                if (close(fd) == -1) {
                    perror("close() failed");
                }
            }
        }
    }
    return SUCCESS_CODE;
}

int main() {
    struct clients_container_t* clients_container = create_clients_container();
    if (clients_container == NULL) {
        perror("creation of clients_container failed()");
        return ERROR_CODE;
    }

    int server_socket;
    int epoll_fd;

    if (create_server_socket(&server_socket) != SUCCESS_CODE) {
        destroy_clients_container(clients_container);
        return ERROR_CODE;
    }

    if (create_epoll(&epoll_fd) != SUCCESS_CODE) {
        destroy_clients_container(clients_container);
        if (close(server_socket) == -1) {
            perror("close() failed");
        }
        return ERROR_CODE;
    }

    if (add_fd_to_epoll_for_read(server_socket, epoll_fd) != SUCCESS_CODE) {
        destroy_clients_container(clients_container);
        if (close(epoll_fd) == -1) {
            perror("close(epoll_fd) failed");
        }
        if (close(server_socket) == -1) {
            perror("close(server_socket) failed");
        }
        return ERROR_CODE;
    }

    on_waiting_on_connections();

    struct epoll_event events[MAX_EVENTS_COUNT];
    while (true) {
        const int count_of_ready_fd = epoll_wait(epoll_fd, events, MAX_EVENTS_COUNT, NO_TIMEOUT);

        if (count_of_ready_fd == -1) {
            perror("epoll_wait() failed");
            break;
        }

        if (handle_ready_sockets(server_socket, epoll_fd, clients_container, events, count_of_ready_fd) != SUCCESS_CODE) {
            break;
        }
    }

    destroy_clients_container(clients_container);

    if (close(epoll_fd) == -1) {
        perror("close(epoll_fd) failed");
    }

    if (close(server_socket) == -1) {
        perror("close(server_socket) failed");
    }

    return ERROR_CODE;
}
