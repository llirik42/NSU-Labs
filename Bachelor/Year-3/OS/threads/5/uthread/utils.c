#define _GNU_SOURCE

#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>
#include <syscall.h>
#include <linux/futex.h>
#include <stdatomic.h>
#include <limits.h>
#include <signal.h>
#include "utils.h"

long futex(volatile uint32_t* uaddr, int futex_op, uint32_t val, const struct timespec* timeout, uint32_t* uaddr2,
           uint32_t val3) {
    return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

long get_page_size() {
    return sysconf(_SC_PAGESIZE);
}

int create_mmap_region(void** region_start_ptr, long size) {
    *region_start_ptr = mmap(
            NULL,
            size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_STACK | MAP_ANONYMOUS,
            -1,
            0
    );

    return (*region_start_ptr) == MAP_FAILED ? -1 : 0;
}

int free_mmap_region(void* region_start, long size) {
    return munmap(region_start, size);
}

void notify_all_on_address(volatile uint32_t* uaddr) {
    futex(uaddr, FUTEX_WAKE, INT_MAX, NULL, NULL, 0);
}

void wait_on_yes_no_address(volatile uint32_t* uaddr, uint32_t value) {
    while (1) {
        uint32_t copy = value;

        if (atomic_compare_exchange_strong(uaddr, &copy, copy)) {
            break;
        }

        const uint32_t invert_value = invert_yes_no(value);
        futex(uaddr, FUTEX_WAIT, invert_value, NULL, NULL, 0);
    }
}

uint32_t invert_yes_no(uint32_t value) {
    return 1 - value;
}

void block_sigalarm() {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGALRM);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
}

void unblock_sigalarm() {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGALRM);
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);
}
