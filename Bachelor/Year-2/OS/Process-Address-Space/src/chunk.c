#include "chunk.h"

#define MIN_CHUNK_LENGTH 1
#define VALIDATION_MASK 7235433442202317929

struct ChunkInfo {
    u_int64_t prev_chunk_length;
    u_int64_t chunk_length;
    u_int64_t validation_data: 63;
    u_int64_t is_free: 1;
};

struct ChunkInfo create_chunk(size_t length) {
    struct ChunkInfo result;

    set_prev_chunk_length(&result, 0);
    set_chunk_length(&result, length);
    set_chunk_free(&result);

    result.validation_data = VALIDATION_MASK;

    return result;
}

void create_chunk_at(void* ptr, size_t length) {
    *((struct ChunkInfo*)(ptr)) = create_chunk(length);
}

size_t get_aligned(size_t value) {
    return value;
}

size_t get_min_chunk_length() {
    return MIN_CHUNK_LENGTH;
}

size_t get_chunk_info_size() {
    return sizeof(struct ChunkInfo);
}

size_t get_prev_chunk_length(const struct ChunkInfo* chunk_ptr) {
    return chunk_ptr->prev_chunk_length;
}

size_t get_chunk_length(const struct ChunkInfo* chunk_ptr) {
    return chunk_ptr->chunk_length;
}

bool is_prev_chunk_free(const struct ChunkInfo* chunk_ptr) {
    return is_chunk_free(((void*)chunk_ptr) - chunk_ptr->prev_chunk_length);
}

bool is_chunk_free(const struct ChunkInfo* chunk_ptr) {
    return chunk_ptr->is_free;
}

bool is_next_chunk_free(const struct ChunkInfo* chunk_ptr) {
    return is_chunk_free(((void*)chunk_ptr) + get_chunk_length(chunk_ptr));
}

bool is_chunk_valid(const struct ChunkInfo* chunk_ptr) {
    return chunk_ptr->validation_data == VALIDATION_MASK;
}

void set_chunk_free(struct ChunkInfo* chunk_ptr) {
    chunk_ptr->is_free = true;
}

void set_chunk_occupied(struct ChunkInfo* chunk_ptr) {
    chunk_ptr->is_free = false;
}

void set_prev_chunk_length(struct ChunkInfo* chunk_ptr, size_t length) {
    chunk_ptr->prev_chunk_length = length;
}

void set_chunk_length(struct ChunkInfo* chunk_ptr, size_t length) {
    chunk_ptr->chunk_length = length;
}

void unite_chunk_with_next(struct ChunkInfo* chunk_ptr) {
    const size_t current_chunk_length = get_chunk_length(chunk_ptr);
    const struct ChunkInfo* next_chunk = ((void*) chunk_ptr) + current_chunk_length;
    set_chunk_length(chunk_ptr, current_chunk_length + get_chunk_length(next_chunk));
}
