#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include "my_heap.h"
#include "chunk.h"

#define PAGES_IN_MY_HEAP 10

#define MIN(A, B) ((A) > (B) ? (B) : (A))

void* my_heap_start = NULL;
void* my_heap_end = NULL;
void* first_free_chunk_ptr = NULL;
size_t my_heap_length;
bool is_my_heap_initialized = false;

int init_my_heap() {
    const long page_size = sysconf(_SC_PAGESIZE);

    if (page_size == -1) {
        return ERROR_CODE;
    }

    my_heap_length = (size_t) (PAGES_IN_MY_HEAP * page_size);

    my_heap_start = mmap(NULL,
                         my_heap_length,
                         PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS,
                         -1,
                         0);

    if (my_heap_start == MAP_FAILED) {
        return ERROR_CODE;
    }

    first_free_chunk_ptr = my_heap_start;
    my_heap_end = my_heap_start + my_heap_length;

    create_chunk_at(my_heap_start, my_heap_length);

    is_my_heap_initialized = true;

    return SUCCESS_CODE;
}

int destroy_my_heap() {
    if (!is_my_heap_initialized || munmap(my_heap_start, my_heap_length) == -1) {
        return ERROR_CODE;
    }

    return SUCCESS_CODE;
}

void* my_malloc(size_t size) {
    if (size == 0 || !is_my_heap_initialized) {
        return NULL;
    }

    const size_t requested_aligned_size = get_aligned(size);

    void* ptr = first_free_chunk_ptr;

    while (1) {
        struct ChunkInfo *chunk_info_ptr = (struct ChunkInfo *) (ptr);
        const size_t chunk_length = get_chunk_length(chunk_info_ptr);

        // If current chunk is occupied
        if (!is_chunk_free(chunk_info_ptr)) {
            ptr += chunk_length;

            if (ptr == my_heap_end) {
                return NULL;
            }

            continue;
        }

        // Number of bytes that can be allocated in this chunk
        const size_t available_size = chunk_length - get_chunk_info_size();

        // If current chunk is free, but too small
        if (available_size < requested_aligned_size) {
            void* next_chunk_ptr = ptr + chunk_length;

            if (next_chunk_ptr == my_heap_end) {
                return NULL;
            }

            ptr = next_chunk_ptr;

            continue;
        }

        set_chunk_occupied(ptr);

        const size_t reserve_size = available_size - requested_aligned_size;
        const size_t offset_for_next_chunk = requested_aligned_size + get_chunk_info_size();
        void* next_chunk_ptr = ptr + offset_for_next_chunk;

        // If current is free and big enough to be split into two chunks (first one - occupied, second one - free)
        if (reserve_size > get_min_chunk_length() + get_chunk_info_size()) {
            set_chunk_length(ptr, offset_for_next_chunk);
            create_chunk_at(next_chunk_ptr, reserve_size);
            set_prev_chunk_length(next_chunk_ptr, offset_for_next_chunk);
        }

        if (ptr == first_free_chunk_ptr) {
            first_free_chunk_ptr = ptr + offset_for_next_chunk;
        }

        return ptr + get_chunk_info_size();
    }
}

int my_free(void *ptr) {
    if (ptr == NULL) {
        return SUCCESS_CODE;
    }

    if (!is_my_heap_initialized || ptr < my_heap_start || ptr > my_heap_end) {
        return ERROR_CODE;
    }

    void* chunk_ptr = ptr - get_chunk_info_size();

    if (!is_chunk_valid(chunk_ptr)) {
        return ERROR_CODE;
    }

    set_chunk_free(chunk_ptr);

    first_free_chunk_ptr = MIN(first_free_chunk_ptr, chunk_ptr);

    // Find start of free-chunks-chain
    void* tmp = chunk_ptr;
    while (is_prev_chunk_free(tmp) && tmp != my_heap_start) {
        const size_t prev_chunk_length = get_prev_chunk_length(tmp);
        tmp -= prev_chunk_length;
    }

    // Unite free chunks in this chain
    while (is_next_chunk_free(tmp) && tmp != my_heap_end) {
        unite_chunk_with_next(tmp);
    }

    return SUCCESS_CODE;
}

int dump_my_heap(FILE* file) {
    if (!is_my_heap_initialized) {
        return ERROR_CODE;
    }

    return fwrite(my_heap_start, sizeof(char), my_heap_length, file) == my_heap_length
        ? SUCCESS_CODE
        : ERROR_CODE;
}
