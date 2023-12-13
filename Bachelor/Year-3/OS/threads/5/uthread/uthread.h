#pragma once

typedef struct uthread_data* uthread_t;

int uthread_create(uthread_t* uthread, void* (* start_routine)(void*), void* arg);

int uthread_join(uthread_t uthread, void** retval);
