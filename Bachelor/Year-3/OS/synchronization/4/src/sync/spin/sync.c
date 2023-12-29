#include <malloc.h>
#include <pthread.h>
#include "../sync.h"

void* create_sync() {
    void* sync = malloc(sizeof(pthread_spinlock_t));

    if (sync == NULL) {
        return NULL;
    }

    if (pthread_spin_init(sync, PTHREAD_PROCESS_PRIVATE)) {
        free(sync);
        return NULL;
    }

    return sync;
}

void destroy_sync(void* sync) {
    pthread_spin_destroy(sync);
}

int rlock_sync(void* sync) {
    return pthread_spin_lock(sync);
}

int wlock_sync(void* sync) {
    return pthread_spin_lock(sync);
}

int unlock_sync(void* sync) {
    return pthread_spin_unlock(sync);
}
