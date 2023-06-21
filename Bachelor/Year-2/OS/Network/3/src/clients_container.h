#pragma once

#include <arpa/inet.h>
#include "utils.h"

struct clients_container_t;

struct clients_container_t* create_clients_container();

void add_client(struct clients_container_t* clients_container, struct client_t client);

struct client_t get_client(const struct clients_container_t* clients_container, int client_socket_fd);

void destroy_clients_container(struct clients_container_t* clients_container);
