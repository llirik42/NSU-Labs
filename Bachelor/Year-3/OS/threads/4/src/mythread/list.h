#pragma once

#include <stdbool.h>
#include "my_thread_data.h"

struct node {
    struct node* next;
    struct node* prev;
    struct mythread_data_t* value;
};

struct list {
    struct node* head;
    struct node* tail;
    uint32_t length;
};

struct list* create_list();
void free_list(struct list* list);

void append(struct list* list, struct mythread_data_t* value);
struct mythread_data_t* find_by_pid(const struct list* list, int pid);
int delete_by_pid(struct list* list, int pid);
bool contains(const struct list* list, struct mythread_data_t* mythread_data);
void free_content(struct list* list);

bool is_empty(const struct list* list);
unsigned int get_length(const struct list* list);
