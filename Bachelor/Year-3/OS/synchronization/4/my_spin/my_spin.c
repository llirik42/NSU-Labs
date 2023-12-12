#include <errno.h>
#include <stdatomic.h>
#include "my_spin.h"

#define SUCCESS 0

#define UNLOCKED 0
#define LOCKED 1

int my_spin_init(my_spinlock_t* lock) {
    *lock = UNLOCKED;
    return SUCCESS;
}

int my_spin_destroy(my_spinlock_t* lock) {
    return SUCCESS;
}

int my_spin_lock(my_spinlock_t* lock) {
    while (1) {
        int tmp = UNLOCKED;

        if (atomic_compare_exchange_strong(lock, &tmp, LOCKED)) {
            break;
        }
    }

    return SUCCESS;
}

int my_spin_trylock(my_spinlock_t* lock) {
    int tmp = UNLOCKED;

    if (atomic_compare_exchange_strong(lock, &tmp, LOCKED)) {
        return SUCCESS;
    }

    return EBUSY;
}

int my_spin_unlock(my_spinlock_t* lock) {
    int tmp = LOCKED;
    atomic_compare_exchange_strong(lock, &tmp, UNLOCKED);
    return SUCCESS;
}
