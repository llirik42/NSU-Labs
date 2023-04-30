#pragma once

#include <aio.h>
#include <bits/types/FILE.h>

#define ERROR_CODE (-1)
#define SUCCESS_CODE 0

// Return value: SUCCESS_CODE — success, ERROR_CODE — can't init my_heap and errno is set appropriately
int init_my_heap();

// Return value: SUCCESS_CODE — success, ERROR_CODE — can't init my_heap and errno is set appropriately
int destroy_my_heap();

// Accepts aligned_size in bytes
// Return value: NULL — not enough memory, non-NULL — else
void* my_malloc(size_t size);

void my_free(void* ptr);

// Return value: SUCCESS_CODE — success, ERROR_CODE — I/O error and errno is set appropriately
int dump_my_heap(FILE* file);
