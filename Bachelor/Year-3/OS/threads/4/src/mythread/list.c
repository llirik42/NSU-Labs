#include <malloc.h>
#include "list.h"

void free_node(struct node* node) {
    free_mythread_data(node->value);
    free(node);
}

struct node* find_node_by_pid(const struct list* list, int pid) {
    struct node* cur = list->head;

    while (cur != NULL) {
        if (cur->value->pid == pid) {
            return cur;
        }
        struct node* next = cur->next;
        cur = next;
    }

    return NULL;
}

void link_nodes(struct node* node1, struct node* node2){
    node1->next = node2;
    node2->prev = node1;
}

struct node* create_Node(struct mythread_data_t* value){
    struct node* ret = (struct node*) malloc(sizeof(struct node));
    ret->value = value;
    ret->next = NULL;
    ret->prev = NULL;

    return ret;
}

struct list* create_list() {
    struct list* ret = (struct list*) malloc(sizeof(struct list));
    ret->head = NULL;
    ret->tail = NULL;
    ret->length = 0;
    return ret;
}

void free_list(struct list* list) {
    if (list == NULL) {
        return;
    }

    free_content(list);
    free(list);
}

void append(struct list* list, struct mythread_data_t* value) {
    struct node* new_node = create_Node(value);

    if (!is_empty(list)){
        link_nodes(list->tail, new_node);
        list->tail = new_node;
    }
    else {
        list->head = new_node;
        list->tail = new_node;
    }

    list->length++;
}

struct mythread_data_t* find_by_pid(const struct list* list, int pid) {
    const struct node* node = find_node_by_pid(list, pid);
    return node == NULL ? NULL : node->value;
}

int delete_by_pid(struct list* list, int pid) {
    struct node* node = find_node_by_pid(list, pid);

    if (node == NULL) {
        return -1;
    }

    if (list->length == 1) {
        list->head = NULL;
        list->tail = NULL;
    }
    else {
        if (list->head == node) {
            list->head = node->next;
            list->head->prev = NULL;
        }
        else if (list->tail == node) {
            list->tail = node->prev;
            list->tail->next = NULL;
        }
        else {
            link_nodes(node->prev, node->next);
        }
    }

    free(node);
    list->length--;

    return 0;
}

bool contains(const struct list* list, struct mythread_data_t* mythread_data) {
    return find_node_by_pid(list, mythread_data->pid) != NULL;
}

void free_content(struct list* list) {
    struct node* cur = list->head;

    while (cur != NULL){
        struct node* next = cur->next;
        free_node(cur);
        cur = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

bool is_empty(const struct list* list) {
    return list->length == 0;
}

unsigned int get_length(const struct list* list) {
    return list->length;
}
