#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mythread/mythread.h"

void* r1(void* arg) {
    sleep(1);
    printf("R1\n");
    return "R1";
}

void* r2(void* arg) {
    sleep(5);
    printf("R2\n");
    return "R2";
}

void* start_routine(void* arg) {
    sleep(1);
    printf("%d-%d\n", getpid(), gettid());
    sleep(1);
    printf("%s\n", (char*)arg);
    sleep(1);
    return "Result";
}

int main() {
//    mythread_t mythread;
//    mythread_create(&mythread, start_routine, "12");
//    void* retval;
//    sleep(5);
//    mythread_join(mythread, &retval);
//    printf("%s\n", (char*)retval);
//    sleep(5);


//    mythread_t mythread;
//    mythread_create(&mythread, start_routine, "12");
//    mythread_detach(mythread);
//    sleep(7);


//    mythread_t mythread;
//    mythread_create(&mythread, start_routine, "12");
//    mythread_exit(NULL);


//    mythread_t mythread_main = mythread_self();
//    mythread_t mythread;
//    mythread_create(&mythread, start_routine, "12");
//    printf("%d\n", mythread_equal(mythread_main, mythread));
//    printf("%d\n", mythread_equal(mythread_main, mythread_main));
//    printf("%d\n", mythread_equal(mythread, mythread));
//    mythread_exit(NULL);


//    mythread_t mythread1;
//    mythread_create(&mythread1, r1, NULL);
//    mythread_t mythread2;
//    mythread_create(&mythread2, r2, NULL);
//    mythread_exit(NULL);


//    mythread_t mythread1;
//    mythread_create(&mythread1, r1, NULL);
//    mythread_t mythread2;
//    mythread_create(&mythread2, r2, NULL);
//
//    void* retval1;
//    mythread_join(mythread1, &retval1);
//    printf("%s\n", (char*)retval1);
//    fflush(stdout);
//
//    void* retval2;
//    mythread_join(mythread2, &retval2);
//    printf("%s\n", (char*)retval2);
//    fflush(stdout);
//
//    sleep(1);


//    mythread_t mythread1;
//    mythread_create(&mythread1, r1, NULL);
//    mythread_t mythread2;
//    mythread_create(&mythread2, r2, NULL);
//    mythread_exit(NULL);
}
