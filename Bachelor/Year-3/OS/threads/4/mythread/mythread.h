#pragma once

#include <stdbool.h>

#define SUCCESS 0

struct mythread_data_t;
typedef struct mythread_data_t* mythread_t;

int mythread_create(mythread_t* mythread, void* (*start_routine)(void*), void* arg);

int mythread_join(mythread_t mythread, void** retval);

int mythread_detach(mythread_t mythread);

bool mythread_equal(mythread_t mythread1, mythread_t mythread2);

mythread_t mythread_self();

void mythread_exit(void* retval);
