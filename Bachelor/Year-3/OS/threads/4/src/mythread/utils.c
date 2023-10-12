#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <stdatomic.h>
#include <limits.h>
#include "utils.h"

long get_page_size() {
    return sysconf(_SC_PAGESIZE);
}

int create_mmap_region(void** region_start_ptr, long stack_size) {
    *region_start_ptr = mmap(
            NULL,
            stack_size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_STACK | MAP_ANONYMOUS,
            -1,
            0
    );

    return (*region_start_ptr) == MAP_FAILED ? -1 : 0;
}

int free_mmap_region(void* region_start, long stack_size) {
    return munmap(region_start, stack_size);
}

int create_thread(int (*routine)(void*), void* stack, void* arg) {
    const int flags =
            CLONE_VM |
            CLONE_FS |
            CLONE_FILES |
            CLONE_SIGHAND |
            CLONE_THREAD |
            CLONE_SYSVSEM;

    return clone(routine, stack, flags, arg);
}

void notify_all_on_address(volatile uint32_t* uaddr) {
    futex(uaddr, FUTEX_WAKE, INT_MAX, NULL, NULL, 0);
}

void wait_on_yes_no_address(volatile uint32_t* uaddr, uint32_t value) {
    const uint32_t invert_value = value == YES ? NO : YES;

    while (1) {
        if (atomic_compare_exchange_strong(uaddr, &value, value)) {
            break;
        }
        futex(uaddr, FUTEX_WAIT, invert_value, NULL, NULL, 0);
    }
}

long futex(volatile uint32_t* uaddr, int futex_op, uint32_t val, const struct timespec* timeout, uint32_t* uaddr2, uint32_t val3) {
    return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}
