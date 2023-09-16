#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define ERROR_CODE 1
#define SUCCESS_CODE 0

void* number() {
    pthread_exit((void*)-100);
}

void* string() {
    pthread_exit("Hello, world!");
}

int main() {
    pthread_t tid;

    const int err1 = pthread_create(&tid, NULL, string, NULL);
    if (err1) {
        fprintf(stderr, "main: pthread_create() failed: %s\n", strerror(err1));
        return ERROR_CODE;
    }

    void* ret_value;
    const int err2 = pthread_join(tid, &ret_value);
    if (err2) {
        fprintf(stderr, "main pthread_join() failed: %s\n", strerror(err2));
        return ERROR_CODE;
    }

    //printf("Returned number from created pthread is %ld\n", (long)ret_value);
    printf("Returned string from created pthread is \"%s\"\n", (char*)ret_value);

    return SUCCESS_CODE;
}
