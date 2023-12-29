#include <malloc.h>
#include "my_spin.h"
#include "../sync.h"

void* create_sync() {
    void* sync = malloc(sizeof(my_spinlock_t));

    if (sync == NULL) {
        return NULL;
    }

    if (my_spin_init(sync)) {
        free(sync);
        return NULL;
    }

    return sync;
}

void destroy_sync(void* sync) {
    my_spin_destroy(sync);
}

int rlock_sync(void* sync) {
    return my_spin_lock(sync);
}

int wlock_sync(void* sync) {
    return my_spin_lock(sync);
}

int unlock_sync(void* sync) {
    return my_spin_unlock(sync);
}
