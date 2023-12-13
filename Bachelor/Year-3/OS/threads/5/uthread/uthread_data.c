#include "utils.h"
#include "uthread_data.h"

void uthread_data_free(struct uthread_data* uthread_data) {
    if (uthread_data) {
        free_mmap_region(uthread_data->stack, uthread_data->stack_size);
    }
}
