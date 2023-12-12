#pragma once

#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include "my_spin/my_spin.h"
#include "my_mutex/my_mutex.h"

struct Node;

int rlock_node(struct Node* node);

int wlock_node(struct Node* node);

int unlock_node(struct Node* node);

struct Node* get_next(const struct Node* node);

void set_next(struct Node* node, struct Node* next_node);

void get_node_value(const struct Node* node, char* output_buffer);


struct Storage;

struct Storage* create_storage(unsigned size);

struct Node* get_head(const struct Storage* storage);
