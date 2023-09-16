#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define ERROR_CODE 1
#define SUCCESS_CODE 0
#define THREADS_COUNT 5

int global_variable = 13;

void *mythread() {
    int local_variable = 16;
    const int local_const_variable = 17;
    static int static_variable = 18;

    printf("mythread [%d %d %d]: Hello from mythread!n\nlocal=(%p, %d)\nlocal_const=%p\nstatic=(%p, %d)\nglobal=(%p, %d)\n\n",
            getpid(), getppid(), gettid(), &local_variable, local_variable, &local_const_variable, &static_variable, \
            static_variable, &global_variable, global_variable);

    while(1) {

    }
}

int main() {
    pthread_t tids[THREADS_COUNT];

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    for (unsigned int i = 0; i < THREADS_COUNT; i++) {
        printf("---------THREAD_CREATION---------\n");
        const int err = pthread_create(&tids[i], NULL, mythread, NULL);
        printf("---------NEW_THREAD_IS_CREATED---------\n");
        if (err) {
            fprintf(stderr, "main: pthread_create() failed: %s\n", strerror(err));
            return ERROR_CODE;
        }
    }

    //pthread_exit(NULL);

    bool join_error = false;
    for (unsigned int i = 0; i < THREADS_COUNT; i++) {
        const int err = pthread_join(tids[i], NULL);
        if (err) {
            fprintf(stderr, "main: join() failed: %s\n", strerror(err));
            join_error = true;
        }
    }

    return join_error ? ERROR_CODE : SUCCESS_CODE;

    //return SUCCESS_CODE;
}
