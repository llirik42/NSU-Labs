#define _GNU_SOURCE

#include <stdatomic.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sched.h>
#include <unistd.h>
#include <linux/futex.h>
#include <limits.h>
#include <stdbool.h>
#include "my_mutex.h"

#define SUCCESS 0

#define UNLOCKED 0
#define LOCKED 1

#define NO_OWNER 0

#define SPIN_ITERATIONS_COUNT 1000000

long futex(volatile uint32_t* uaddr, int futex_op, uint32_t val, const struct timespec* timeout, uint32_t* uaddr2, uint32_t val3) {
    return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

bool lock_mutex(my_mutex_t* mutex) {
    uint32_t unlocked = UNLOCKED;

    if (atomic_compare_exchange_strong(&mutex->lock, &unlocked, LOCKED)) {
        atomic_store(&mutex->owner, gettid());
        return true;
    }

    return false;
}

void wait_until_unlocked_and_lock(my_mutex_t* mutex) {
    for (unsigned int i = 0; i < SPIN_ITERATIONS_COUNT; i++) {
        if (lock_mutex(mutex)) {
            return;
        }
    }

    while (1) {
        if (lock_mutex(mutex)) {
            return;
        }
        futex(&mutex->lock, FUTEX_WAIT, LOCKED, NULL, NULL, 0);
    }
}

void unlock_mutex(my_mutex_t* mutex) {
    atomic_store(&mutex->owner, NO_OWNER);
    atomic_store(&mutex->lock, UNLOCKED);
    futex(&mutex->lock, FUTEX_WAKE, INT_MAX, NULL, NULL, 0);
}

int my_mutex_init(my_mutex_t* mutex, enum my_mutex_kind kind) {
    mutex->lock = UNLOCKED;
    mutex->count = 0;
    mutex->owner = NO_OWNER;
    mutex->kind = kind;
    return SUCCESS;
}

int my_mutex_destroy(my_mutex_t* mutex) {
    uint32_t locked = LOCKED;

    if (atomic_compare_exchange_strong(&mutex->lock, &locked, LOCKED)) {
        return EBUSY;
    }

    return SUCCESS;
}

int my_mutex_lock(my_mutex_t* mutex) {
    const int tid = gettid();

    switch (mutex->kind) {
        case FAST:
            wait_until_unlocked_and_lock(mutex);
            return SUCCESS;

        case ERROR_CHECKING:
            if (mutex->owner != tid) {
                wait_until_unlocked_and_lock(mutex);
                return SUCCESS;
            }
            return EDEADLK;

        case RECURSIVE:
            if (mutex->owner != tid) {
                wait_until_unlocked_and_lock(mutex);
            }
            mutex->count++;
            return SUCCESS;

        default: return EINVAL;
    }
}

int my_mutex_trylock(my_mutex_t* mutex) {
    switch (mutex->kind) {
        case FAST:

        case ERROR_CHECKING:
            if (lock_mutex(mutex)) {
                return SUCCESS;
            }
            return EBUSY;

        case RECURSIVE:
            if (lock_mutex(mutex)) {
                mutex->count++;
                return SUCCESS;
            }
            return EBUSY;

        default: return EINVAL;
    }
}

int my_mutex_unlock(my_mutex_t* mutex) {
    const int tid = gettid();

    switch (mutex->kind) {
        case FAST:
            if (mutex->owner == tid) {
                unlock_mutex(mutex);
            }
            return SUCCESS;

        case ERROR_CHECKING:
            if (mutex->owner == tid) {
                unlock_mutex(mutex);
                return SUCCESS;
            }
            return EPERM;

        case RECURSIVE:
            if (mutex->owner == tid) {
                mutex->count--;
                if (mutex->count == 0) {
                    unlock_mutex(mutex);
                }
            }
            return SUCCESS;

        default: return EINVAL;
    }
}
