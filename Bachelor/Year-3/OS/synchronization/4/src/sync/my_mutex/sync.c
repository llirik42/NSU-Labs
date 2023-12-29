#include <malloc.h>
#include "my_mutex.h"
#include "../sync.h"

void* create_sync() {
    void* sync = malloc(sizeof(my_mutex_t));

    if (sync == NULL) {
        return NULL;
    }

    if (my_mutex_init(sync, FAST)) {
        free(sync);
        return NULL;
    }

    return sync;
}

void destroy_sync(void* sync) {
    my_mutex_destroy(sync);
}

int rlock_sync(void* sync) {
    return my_mutex_lock(sync);
}

int wlock_sync(void* sync) {
    return my_mutex_lock(sync);
}

int unlock_sync(void* sync) {
    return my_mutex_unlock(sync);
}
