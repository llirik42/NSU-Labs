#pragma once

#include <stdint.h>
#include <ucontext.h>

struct uthread_data {
    unsigned int tid;
    void* stack;
    long stack_size;

    void* (* routine)(void*);

    ucontext_t ctx;
    void* arg;
    void* retval;
    uint32_t exited;
    uint32_t joined;
};

void uthread_data_free(struct uthread_data* uthread_data);
