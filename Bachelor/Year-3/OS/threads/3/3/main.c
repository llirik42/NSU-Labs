#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define ERROR_CODE 1

typedef struct {
    int important_number;

    char* important_char_pointer;
} my_super_struct_3000;

my_super_struct_3000 s = {.important_number = 13, .important_char_pointer = "Important string!"};

void print_my_super_struct_3000(const my_super_struct_3000* _s) {
    printf("my_super_struct_3000(important_number=%d, important_char_pointer=\"%s\")\n", _s->important_number, _s->important_char_pointer);
}

void* routine(void* arg) {
    sleep(2);
    print_my_super_struct_3000((my_super_struct_3000*)arg);
    return NULL;
}

int main() {
    printf("%d\n", getpid());
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

    // my_super_struct_3000 s = {.important_number = 13, .important_char_pointer = "Important string!"};

    // static my_super_struct_3000 s = {.important_number = 13, .important_char_pointer = "Important string!"};

    // my_super_struct_3000* s = malloc(sizeof(my_super_struct_3000));
    // s->important_number = 13;
    // s->important_char_pointer = "Important string!";

    pthread_t tid;
    printf("CREATION\n");
    const int err3 = pthread_create(&tid, &attrs, routine, &s);
    if (err3) {
        fprintf(stderr, "main: pthread_create() failed: %s\n", strerror(err3));
        pthread_attr_destroy(&attrs);
        return ERROR_CODE;
    }

    sleep(5);
    printf("Main exiting ...\n");
    pthread_exit(NULL);
}
