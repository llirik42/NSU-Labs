#include <stdio.h>
#include "uthread/uthread.h"
#include "uthread/scheduler.h"

void* routine1(void* arg) {
    for (int i = 0; i < 50000000; i++) {

    }

    printf("Hi, %s\n", arg);
    return "Result!";
}

int main(int argc, char* argv[]) {
    scheduler_init(1000);

    uthread_t tid1, tid2, tid3, tid4, tid5;

    uthread_create(&tid1, routine1, "1");
    uthread_create(&tid2, routine1, "2");
    uthread_create(&tid3, routine1, "3");
    uthread_create(&tid4, routine1, "4");
    uthread_create(&tid5, routine1, "5");

    void* val1;
    uthread_join(tid1, &val1);
    printf("%s\n", val1);

    void* val2;
    uthread_join(tid2, &val2);

    void* val3;
    uthread_join(tid3, &val3);

    void* val4;
    uthread_join(tid4, &val4);

    void* val5;
    uthread_join(tid5, &val5);

    scheduler_shutdown();
}
