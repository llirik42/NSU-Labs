#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define ERROR_CODE 1

void* routine() {
    printf("tid%d pthread_self()=%ld\n", gettid(), pthread_self());
    //pthread_detach(pthread_self());
    return NULL;
}

int main() {
    pthread_attr_t attrs;

    const int err1 = pthread_attr_init(&attrs);
    if (err1) {
        fprintf(stderr, "main: pthread_attr_init() failed: %s\n", strerror(err1));
        pthread_attr_destroy(&attrs);
        return ERROR_CODE;
    }

    const int err2 = pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    if (err2) {
        fprintf(stderr, "main: pthread_attr_setdetachstate() failed: %s\n", strerror(err2));
        pthread_attr_destroy(&attrs);
        return ERROR_CODE;
    }

    while(1) {
        pthread_t tid;

        const int err = pthread_create(&tid, &attrs, routine, NULL);
        if (err) {
            fprintf(stderr, "main: pthread_create() failed: %s\n", strerror(err));
            pthread_attr_destroy(&attrs);
            return ERROR_CODE;
        }
    }
}
