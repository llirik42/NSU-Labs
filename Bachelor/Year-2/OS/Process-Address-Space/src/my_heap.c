#include <stddef.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include "my_heap.h"
#include "chunk.h"

#define PAGES_IN_MY_HEAP 10

const size_t CHUNK_INFO_SIZE = sizeof(struct ChunkInfo);
void* my_heap_start = NULL;
void* my_heap_end = NULL;
size_t my_heap_length;

int init_my_heap() {
    const long page_size = sysconf(_SC_PAGESIZE);

    if (page_size == -1) {
        return ERROR_CODE;
    }

    my_heap_length = PAGES_IN_MY_HEAP * page_size;

    my_heap_start = mmap(NULL,
                         my_heap_length,
                         PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS,
                         -1,
                         0);

    if (my_heap_start == MAP_FAILED) {
        return ERROR_CODE;
    }

    my_heap_end = my_heap_start + my_heap_length;

    create_chunk_info_at(my_heap_start, my_heap_length);

    return SUCCESS_CODE;
}

int destroy_my_heap() {
    if (munmap(my_heap_start, my_heap_length) == -1) {
        return ERROR_CODE;
    }

    return SUCCESS_CODE;
}

void* my_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    const size_t aligned_size = get_aligned(size);

    void* ptr = my_heap_start;

    while (1) {
        struct ChunkInfo *chunk_info_ptr = (struct ChunkInfo *) (ptr);
        const size_t chunk_length = get_chunk_length(chunk_info_ptr);

        // If current chunk is occupied
        if (is_chunk_occupied(chunk_info_ptr)) {
            ptr += chunk_length;

            if (ptr == my_heap_end) {
                return NULL;
            }

            continue;
        }

        const size_t available_size = chunk_length - CHUNK_INFO_SIZE;

        // If current chunk is free, but too small
        if (available_size < aligned_size) {
            if (ptr + chunk_length == my_heap_end) {
                return NULL;
            }

            ptr += chunk_length;

            continue;
        }

        set_chunk_occupied(ptr);

        // If current is free and big enough to be split into two chunks (first one - occupied, second one - free)
        if (available_size - aligned_size > MIN_CHUNK_LENGTH + CHUNK_INFO_SIZE) {
            set_chunk_length(ptr, aligned_size + CHUNK_INFO_SIZE);
            create_chunk_info_at(ptr + aligned_size + CHUNK_INFO_SIZE, available_size - aligned_size);
        }

        return ptr + CHUNK_INFO_SIZE;
    }
}

void my_free(void *ptr) {
    if (ptr != NULL) {
        set_chunk_free(ptr - sizeof(struct ChunkInfo));
    }
}

int dump_my_heap(FILE* file) {
    for (size_t i = 0; i < my_heap_length; i++) {
        if (fprintf(file, "%c",  ((char*)(my_heap_start))[i]) < 0) {
            return ERROR_CODE;
        }
    }

    return fflush(file);
}
