#include <limits.h>
#include <stdatomic.h>
#include <string.h>
#include "gc_thread.h"
#include "utils.h"

#define PAGES_IN_GC_STACK 10

struct list* zombie_threads_list;

int gc(__attribute__((unused)) void* arg) {
    uint32_t zero = 0;

    while (1) {
        if (atomic_compare_exchange_strong(&(zombie_threads_list->length), &zero, 0)) {
            futex(&(zombie_threads_list->length), FUTEX_WAIT, 0, NULL, NULL, 0);
        }

        free_content(zombie_threads_list);
    }
}

int create_gc_thread(struct list* zombie_threads) {
    zombie_threads_list = zombie_threads;

    const long page_size = get_page_size();
    const long stack_size = page_size * PAGES_IN_GC_STACK;

    void* stack;
    if (create_mmap_region(&stack, stack_size) == -1) {
        return -1;
    }

    return create_thread(gc, stack + stack_size, NULL);
}

void notify_gc_thread() {
    futex(&(zombie_threads_list->length), FUTEX_WAKE, INT_MAX, NULL, NULL, 0);
}
