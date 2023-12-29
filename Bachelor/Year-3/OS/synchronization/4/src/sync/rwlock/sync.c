#include <malloc.h>
#include <pthread.h>
#include "../sync.h"

void* create_sync() {
    void* sync = malloc(sizeof(pthread_rwlock_t));

    if (sync == NULL) {
        return NULL;
    }

    if (pthread_rwlock_init(sync, NULL)) {
        free(sync);
        return NULL;
    }

    return sync;
}

void destroy_sync(void* sync) {
    pthread_rwlock_destroy(sync);
}

int rlock_sync(void* sync) {
    return pthread_rwlock_rdlock(sync);
}

int wlock_sync(void* sync) {
    return pthread_rwlock_wrlock(sync);
}

int unlock_sync(void* sync) {
    return pthread_rwlock_unlock(sync);
}
