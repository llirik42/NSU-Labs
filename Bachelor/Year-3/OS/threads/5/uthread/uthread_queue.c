#include <malloc.h>
#include <stdbool.h>
#include "uthread_queue.h"

struct node {
    struct uthread_data* value;
    struct node* next;
};

struct uthread_queue {
    struct node* head;
    struct node* tail;
    unsigned int length;
};

bool does_node_have_tid(const struct node* n, unsigned int tid) {
    return n->value->tid == tid;
}

struct node* create_node(struct uthread_data* value) {
    struct node* ret = malloc(sizeof(struct node));

    if (ret != NULL) {
        ret->value = value;
    }

    return ret;
}

struct uthread_queue* uthread_queue_create() {
    struct uthread_queue* ret = malloc(sizeof(struct uthread_queue));

    // Don't have enough memory
    if (ret == NULL) {
        return NULL;
    }

    ret->head = NULL;
    ret->tail = NULL;
    ret->length = 0;

    return ret;
}

void uthread_queue_destroy(struct uthread_queue* uthread_queue) {
    struct node* current_node = uthread_queue->head;

    while (current_node != NULL) {
        struct node* next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }

    free(uthread_queue);
}

int uthread_queue_append(struct uthread_queue* uthread_queue, struct uthread_data* uthread_data) {
    struct node* new_node = create_node(uthread_data);
    if (new_node == NULL) {
        return UTHREAD_QUEUE_NO_MEMORY;
    }

    struct node* tail = uthread_queue->tail;

    // Queue is empty
    if (tail == NULL) {
        uthread_queue->head = new_node;
    } else {
        uthread_queue->tail->next = new_node;
    }

    uthread_queue->tail = new_node;
    uthread_queue->length++;

    return UTHREAD_QUEUE_SUCCESS;
}

struct uthread_data* uthread_queue_get(const struct uthread_queue* uthread_queue, unsigned int index) {
    struct node* head = uthread_queue->head;
    if (head == NULL) {
        return NULL;
    }

    struct node* current_node = head;

    for (unsigned int i = 0; i < index; i++) {
        current_node = current_node->next;

        if (current_node == NULL) {
            return NULL;
        }
    }

    return current_node->value;
}

void uthread_queue_remove(struct uthread_queue* uthread_queue, unsigned int tid) {
    struct node* head = uthread_queue->head;

    // Queue is empty, so we cannot remove anything
    if (head == NULL) {
        return;
    }

    // If first node is the node we want to remove
    if (does_node_have_tid(head, tid)) {
        uthread_queue->head = head->next;
        free(head);
        uthread_queue->length--;
        return;
    }

    struct node* previous_node = head;
    struct node* current_node = previous_node->next;

    while (current_node != NULL) {
        if (does_node_have_tid(current_node, tid)) {
            previous_node->next = current_node->next;
            if (current_node == uthread_queue->tail) {
                uthread_queue->tail = previous_node;
            }
            free(current_node);
            uthread_queue->length--;
            return;
        }

        previous_node = current_node;
        current_node = current_node->next;
    }
}

unsigned int uthread_queue_length(const struct uthread_queue* uthread_queue) {
    return uthread_queue->length;
}