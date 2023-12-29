#include <pthread.h>
#include <malloc.h>
#include "../sync.h"

void* create_sync() {
    void* sync = malloc(sizeof(pthread_mutex_t));

    if (sync == NULL) {
        return NULL;
    }

    if (pthread_mutex_init(sync, NULL)) {
        free(sync);
        return NULL;
    }

    return sync;
}

void destroy_sync(void* sync) {
    pthread_mutex_destroy(sync);
}

int rlock_sync(void* sync) {
    return pthread_mutex_lock(sync);
}

int wlock_sync(void* sync) {
    return pthread_mutex_lock(sync);
}

int unlock_sync(void* sync) {
    return pthread_mutex_unlock(sync);
}
