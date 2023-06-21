#include <malloc.h>
#include "clients_container.h"

#define CONTAINER_SIZE 512

struct clients_container_t {
    struct client_t clients[CONTAINER_SIZE];
};

struct clients_container_t* create_clients_container() {
    return malloc(sizeof(struct clients_container_t));
}

void add_client(struct clients_container_t* clients_container, struct client_t client) {
    clients_container->clients[client.socket_fd] = client;
}

struct client_t get_client(const struct clients_container_t* clients_container, int client_socket_fd) {
    return clients_container->clients[client_socket_fd];
}

void destroy_clients_container(struct clients_container_t* clients_container) {
    free(clients_container);
}
