#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define ERROR_CODE 1

void free_memory(void* ptr) {
    free(ptr);
    printf("I freed memory!\n");
}

void* routine() {
    const int err = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    if (err) {
        fprintf(stderr, "child: pthread_setcanceltype() failed: %s\n", strerror(err));
        return NULL;
    }

    // unsigned int i = 0;
    // for (unsigned int i = 0; ; i++) {
    // 	//printf("%s, %d\n", "NEPON", i);
    // }

    char* ptr = malloc(100);
    pthread_cleanup_push(free_memory, (void*)ptr)
        char* str = "hello, world!";
        for (int i = 0; i < strlen(str); i++) {
            ptr[i] = str[i];
        }
        ptr[strlen(str)] = '\0';
        while(1) {
            printf("%s\n", ptr);
        }
    pthread_cleanup_pop(0);
}

int main() {
    pthread_t tid;

    const int err1 = pthread_create(&tid, NULL, routine, NULL);
    if (err1) {
        fprintf(stderr, "main: pthread_create() failed: %s\n", strerror(err1));
        return ERROR_CODE;
    }

    sleep(3);

    const int err2 = pthread_cancel(tid);
    if (err2) {
        fprintf(stderr, "main: pthread_cancel() failed: %s\n", strerror(err2));
        return ERROR_CODE;
    }

    const int err3 = pthread_join(tid, NULL);
    if (err3) {
        fprintf(stderr, "main: pthread_join() failed: %s\n", strerror(err3));
        return ERROR_CODE;
    }
}
