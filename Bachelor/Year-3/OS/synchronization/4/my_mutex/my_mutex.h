#pragma once

#include <sched.h>
#include <stdint.h>

enum my_mutex_kind {
    FAST,
    ERROR_CHECKING,
    RECURSIVE
};

typedef struct {
    volatile uint32_t lock;
    unsigned int count;
    volatile uint32_t owner;
    enum my_mutex_kind kind;
} my_mutex_t;

int my_mutex_init(my_mutex_t* mutex, enum my_mutex_kind kind);

int my_mutex_destroy(my_mutex_t* mutex);

int my_mutex_lock(my_mutex_t* mutex);

int my_mutex_trylock(my_mutex_t* mutex);

int my_mutex_unlock(my_mutex_t* mutex);
