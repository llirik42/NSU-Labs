#pragma once

#include <linux/futex.h>
#include <stdint.h>
#include <sys/time.h>

#define YES 1
#define NO 0

long get_page_size();

int create_mmap_region(void** region_start_ptr, long stack_size);

int free_mmap_region(void* region_start, long stack_size);

int create_thread(int (*routine)(void*), void* stack, void* arg);

void notify_all_on_address(volatile uint32_t* uaddr);

void wait_on_yes_no_address(volatile uint32_t* uaddr, uint32_t value);

long futex(volatile uint32_t* uaddr, int futex_op, uint32_t val, const struct timespec* timeout, uint32_t* uaddr2, uint32_t val3);
