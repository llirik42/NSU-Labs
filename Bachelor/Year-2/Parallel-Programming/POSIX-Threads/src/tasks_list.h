#pragma once

#include <stdbool.h>
#include "task.h"

typedef struct {
    task_t* tasks;
    size_t capacity;
    size_t current_task_index;
    size_t new_task_index;
} tasks_list_t;

void init_tasks_list(tasks_list_t* tasks_list);

void add_task(tasks_list_t* tasks_list, task_t task);

bool reached_end(const tasks_list_t* tasks_list);

size_t get_remaining_tasks_count(const tasks_list_t* tasks_list);

task_t get_task(tasks_list_t* tasks_list);

void free_tasks_list(tasks_list_t* tasks_list);
