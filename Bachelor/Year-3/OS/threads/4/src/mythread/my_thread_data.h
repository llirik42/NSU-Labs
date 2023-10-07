#pragma once

#include <stdint-gcc.h>
#include <stdbool.h>

struct mythread_data_t {
    int id;
    int pid;
    void* stack;
    long stack_size;
    void* (*start_routine)(void*);
    void* arg;
    void* retval;
    volatile uint32_t exited;
    volatile uint32_t joined;
    volatile uint32_t detached;
};

void free_mythread_data(struct mythread_data_t* mythread_data);
