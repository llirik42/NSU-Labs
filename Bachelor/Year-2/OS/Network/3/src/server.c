#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#include "config.h"

#define BACKLOG 5
#define MAX_EVENTS_COUNT 64
#define MAX_TOTAL_CLIENTS 512 
#define EPOLL_WAIT_TIMEOUT_MS 500

int handle_connection(int client_socket_fd, const struct sockaddr_in* client_sockaddr) {
    char message[MAX_MESSAGE_LENGTH] = {'\0'};

    while (true) {
        const ssize_t recv_count = recv(client_socket_fd, message, MAX_MESSAGE_LENGTH, MSG_DONTWAIT);

        message[recv_count] = '\0';

        if (recv_count == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return SUCCESS_CODE;
            }

            perror("recv() failed");
            return ERROR_CODE;
        }

        if (recv_count == 0) {
            if (errno) {
                printf("Connection with %s:%d is lost\n",
                       inet_ntoa(client_sockaddr->sin_addr),
                       ntohs(client_sockaddr->sin_port));
                return ERROR_CODE;
            }
            return SUCCESS_CODE;
        }

        printf("Server received from %s:%d. Message: %s\n",
               inet_ntoa(client_sockaddr->sin_addr),
               ntohs(client_sockaddr->sin_port),
               message);

        const ssize_t send_count = send(client_socket_fd, message, recv_count, 0);

        if (send_count == -1) {
            perror("sendto() failed");
            return ERROR_CODE;
        }

        printf("Server send: %s\n", message);
    }
}

int handle_client(int client_socket_fd, const struct sockaddr_in* client_sockaddr, struct sockaddr_in* client_sockaddrs,
                  int epoll_fd) {

    if (client_socket_fd == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("accept() failed");
            return ERROR_CODE;
        }

        return SUCCESS_CODE;
    }
    else {
        printf("New client: %s:%d\n",
               inet_ntoa(client_sockaddr->sin_addr),
               ntohs(client_sockaddr->sin_port));

        client_sockaddrs[client_socket_fd] = *client_sockaddr;

        struct epoll_event configuration_epoll_event = {
                .events = EPOLLIN,
                .data.fd = client_socket_fd
        };

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket_fd, &configuration_epoll_event) == -1) {
            perror("epoll_ctl(add) failed");
            return ERROR_CODE;
        }

        return SUCCESS_CODE;
    }
}

int main() {
    const int server_socket = socket(AF_INET, SOCK_STREAM, 0); // Server socket

    if (server_socket == -1) {
        perror("Socket creation failed");
        return ERROR_CODE;
    }

    const struct sockaddr_in server_sockaddr = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = htonl(INADDR_ANY),
            .sin_port = htons(PORT)
    };

    if (bind(server_socket, (const struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr)) == -1) {
        perror("bind() failed");
        if (close(server_socket) == -1) {
            perror("close() failed");
        }
        return ERROR_CODE;
    }

    if (listen(server_socket, BACKLOG) == -1) {
        perror("listen() failed");
        if (close(server_socket) == -1) {
            perror("close() failed");
        }
        return ERROR_CODE;
    }

    // Creation of epoll-instance
    const int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_creation failed");
        if (close(server_socket) == -1) {
            perror("close() failed");
        }
        return ERROR_CODE;
    }

    struct sockaddr_in client_sockaddrs[MAX_TOTAL_CLIENTS]; // client_fd -> client_sockaddr_in
    struct sockaddr_in client_sockaddr;
    socklen_t socklen = sizeof(client_sockaddr);
    struct epoll_event events[MAX_EVENTS_COUNT];

    printf("Waiting on connections ...\n");

    // Waiting for first connection
    const int first_client_socket_fd = accept(server_socket, (struct sockaddr*) &client_sockaddr, &socklen);

    if (handle_client(first_client_socket_fd, &client_sockaddr, client_sockaddrs, epoll_fd) == ERROR_CODE) {
        perror("handle_client() failed");
    }
    else {
        // Setting server socket to non-blocking mode
        if (fcntl(server_socket, F_SETFL, fcntl(server_socket, F_GETFL) | O_NONBLOCK) == -1) {
            perror("setting server socket into non-blocking mode failed");
        } else {
            while (true) {
                const int client_socket_fd = accept(server_socket, (struct sockaddr *) &client_sockaddr, &socklen);

                if (handle_client(client_socket_fd, &client_sockaddr, client_sockaddrs, epoll_fd) == ERROR_CODE) {
                    perror("handle_client() failed");
                    break;
                }

                const int count_of_ready_fd = epoll_wait(epoll_fd, events, MAX_EVENTS_COUNT, EPOLL_WAIT_TIMEOUT_MS);

                if (count_of_ready_fd == -1) {
                    perror("epoll_wait() failed");
                    break;
                }

                // Handling connections with events
                for (int i = 0; i < count_of_ready_fd; i++) {
                    const struct epoll_event event = events[i];
                    const int fd = event.data.fd;
                    const struct sockaddr_in sockaddr = client_sockaddrs[fd];

                    if (handle_connection(fd, &sockaddr) == ERROR_CODE) {
                        if (close(fd) == -1) {
                            perror("close() failed");
                        }
                    }
                }
            }
        }
    }

    if (close(epoll_fd) == -1) {
        perror("close(epoll_fd) failed");
    }

    if (close(server_socket) == -1) {
        perror("close(server_socket) failed");
    }

    return ERROR_CODE;
}
