#include <malloc.h>
#include "tasks_list.h"
#include "config.h"

void init_tasks_list(tasks_list_t* tasks_list) {
    tasks_list->current_task_index = 0;
    tasks_list->new_task_index = 0;
    tasks_list->capacity = TASKS_IN_LIST_PER_PROCESS;
    tasks_list->tasks = calloc(tasks_list->capacity, sizeof(task_t));
}

void add_task(tasks_list_t* tasks_list, task_t task) {
    if (tasks_list->new_task_index == tasks_list->capacity) {
        tasks_list->capacity *= 2;
        tasks_list->tasks = reallocarray(tasks_list->tasks, tasks_list->capacity, sizeof(task));
    }
    tasks_list->tasks[tasks_list->new_task_index++] = task;
}

bool reached_end(const tasks_list_t* tasks_list) {
    return get_remaining_tasks_count(tasks_list) == 0;
}

size_t get_remaining_tasks_count(const tasks_list_t* tasks_list) {
    return tasks_list->new_task_index - tasks_list->current_task_index;
}

task_t get_task(tasks_list_t* tasks_list) {
    return tasks_list->tasks[tasks_list->current_task_index++];
}

void free_tasks_list(tasks_list_t* tasks_list) {
    free(tasks_list->tasks);
}
