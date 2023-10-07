#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "my_thread_data.h"

void free_mythread_data(struct mythread_data_t* mythread_data) {
    int id = mythread_data->id;
    const int code = free_mmap_region(mythread_data->stack, mythread_data->stack_size);
    printf("Resources for thread-%d are freed with %s\n", id, strerror(code));
}
