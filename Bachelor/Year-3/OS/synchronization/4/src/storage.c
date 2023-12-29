#include <stdatomic.h>
#include "utils.h"
#include "storage.h"
#include "sync/sync.h"

struct Node {
    char value[MAX_STRING_LENGTH + 1];
    struct Node* next;
    void* sync;
};

struct Node* create_node(const char* value) {
    struct Node* node = malloc(sizeof(struct Node));
    if (node == NULL) {
        return NULL;
    }

    strcpy(node->value, value);

    node->next = NULL;
    node->sync = create_sync();

    if (node->sync == NULL) {
        free(node);
        return NULL;
    }

    return node;
}

void destroy_node(struct Node* node) {
    destroy_sync(node->sync);
    free(node);
}

int rlock_node(struct Node* node) {
    return rlock_sync(node->sync);
}

int wlock_node(struct Node* node) {
    return wlock_sync(node->sync);
}

int unlock_node(struct Node* node) {
    if (node != NULL) {
        return unlock_sync(node->sync);
    }

    return 0;
}

struct Node* get_next(const struct Node* node) {
    return node->next;
}

void set_next(struct Node* node, struct Node* next_node) {
    atomic_store(&node->next, next_node);
}

void get_node_value(const struct Node* node, char* output_buffer) {
    strcpy(output_buffer, node->value);
}


struct Storage {
    struct Node* first;
    void* sync;
};

void destroy_storage(struct Storage* storage) {
    struct Node* current_node = storage->first;

    while (current_node != NULL) {
        struct Node* next_node = current_node->next;
        destroy_node(current_node);
        current_node = next_node;
    }

    free(storage);
}

struct Storage* create_storage(unsigned size) {
    struct Storage* storage = malloc(sizeof(struct Storage));
    if (storage == NULL) {
        return NULL;
    }
    storage->first = NULL;
    storage->sync = create_sync();
    if (storage->sync == NULL) {
        free(storage);
        return NULL;
    }

    char buffer[MAX_STRING_LENGTH + 1];

    // Creating head-node
    generate_random_string(buffer);
    struct Node* current_node = create_node(buffer);

    if (current_node == NULL) {
        destroy_storage(storage);
        return NULL;
    }

    storage->first = current_node;

    for (unsigned int i = 0; size != 0 && i < size - 1; i++) {
        generate_random_string(buffer);
        struct Node* new_node = create_node(buffer);

        if (new_node == NULL) {
            destroy_storage(storage);
            return NULL;
        }

        current_node->next = new_node;
        current_node = new_node;
    }

    return storage;
}

struct Node* get_head(const struct Storage* storage) {
    return storage->first;
}

int rlock_storage(struct Storage* storage) {
    return rlock_sync(storage->sync);
}

int wlock_storage(struct Storage* storage) {
    return wlock_sync(storage->sync);
}

int unlock_storage(struct Storage* storage) {
    return unlock_sync(storage->sync);
}
