#pragma once

#include <stdint.h>

#define YES 1
#define NO 0

long get_page_size();

int create_mmap_region(void** region_start_ptr, long size);

int free_mmap_region(void* region_start, long size);

void notify_all_on_address(volatile uint32_t* uaddr);

void wait_on_yes_no_address(volatile uint32_t* uaddr, uint32_t value);

uint32_t invert_yes_no(uint32_t value);

void block_sigalarm();

void unblock_sigalarm();
