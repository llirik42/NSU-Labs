#include <stdatomic.h>
#include "utils.h"
#include "storage.h"

struct Node {
    char value[MAX_STRING_LENGTH + 1];
    struct Node* next;

    #ifdef SPIN
        pthread_spinlock_t spin;
    #elif defined(MUTEX)
        pthread_mutex_t mutex;
    #elif defined(MY_SPIN)
        my_spinlock_t my_spin;
    #elif defined(MY_MUTEX)
        my_mutex_t my_mutex;
    #else
        pthread_rwlock_t rwlock;
    #endif
};

struct Node* create_node(const char* value) {
    struct Node* node = malloc(sizeof(struct Node));
    if (node == NULL) {
        return NULL;
    }

    strcpy(node->value, value);

    node->next = NULL;

    #ifdef SPIN
        if (pthread_spin_init(&node->spin, PTHREAD_PROCESS_PRIVATE)) {
            free(node);
            return NULL;
        }
    #elif defined(MUTEX)
        if (pthread_mutex_init(&node->mutex, NULL)) {
            free(node);
            return NULL;
        }
    #elif defined(MY_SPIN)
        if (my_spin_init(&node->my_spin)) {
            free(node);
            return NULL;
        }
    #elif defined(MY_MUTEX)
        if (my_mutex_init(&node->my_mutex, FAST)) {
            free(node);
            return NULL;
        }
    #else
        if (pthread_rwlock_init(&node->rwlock, NULL)) {
            free(node);
            return NULL;
        }
    #endif

    return node;
}

int rlock_node(struct Node* node) {
    #ifdef SPIN
        return pthread_spin_lock(&node->spin);
    #elif defined(MUTEX)
        return pthread_mutex_lock(&node->mutex);
    #elif defined(MY_SPIN)
        return my_spin_lock(&node->my_spin);
    #elif defined(MY_MUTEX)
        return my_mutex_lock(&node->my_mutex);
    #else
        return pthread_rwlock_rdlock(&node->rwlock);
    #endif
}

int wlock_node(struct Node* node) {
    #ifdef SPIN
        return pthread_spin_lock(&node->spin);
    #elif defined(MUTEX)
        return pthread_mutex_lock(&node->mutex);
    #elif defined(MY_SPIN)
        return my_spin_lock(&node->my_spin);
    #elif defined(MY_MUTEX)
        return my_mutex_lock(&node->my_mutex);
    #else
        return pthread_rwlock_wrlock(&node->rwlock);
    #endif
}

int unlock_node(struct Node* node) {
    if (node != NULL) {
        #ifdef SPIN
            return pthread_spin_unlock(&node->spin);
        #elif defined(MUTEX)
            return pthread_mutex_unlock(&node->mutex);
        #elif defined(MY_SPIN)
            return my_spin_unlock(&node->my_spin);
        #elif defined(MY_MUTEX)
            return my_mutex_unlock(&node->my_mutex);
        #else
            return pthread_rwlock_unlock(&node->rwlock);
        #endif
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
};

void destroy_storage(struct Storage* storage) {
    struct Node* current_node = storage->first;

    while (current_node != NULL) {
        struct Node* next_node = current_node->next;
        free(current_node);
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
