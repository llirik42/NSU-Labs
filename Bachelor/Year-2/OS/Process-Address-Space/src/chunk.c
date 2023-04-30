#include "chunk.h"

struct ChunkInfo create_chunk_info(size_t length) {
    struct ChunkInfo result;

    set_chunk_length(&result, length);
    set_chunk_free(&result);

    return result;
}

void create_chunk_info_at(void* ptr, size_t length) {
    *((struct ChunkInfo*)(ptr)) = create_chunk_info(length);
}

void set_chunk_free(struct ChunkInfo* chunk_info) {
    chunk_info->is_free = true;
}

void set_chunk_occupied(struct ChunkInfo* chunk_info) {
    chunk_info->is_free = false;
}

void set_chunk_length(struct ChunkInfo* chunk_info, size_t length) {
    chunk_info->data = get_aligned(length) / 2;
}

bool is_chunk_occupied(const struct ChunkInfo* chunk_info) {
    return !(chunk_info->is_free);
}

size_t get_chunk_length(const struct ChunkInfo* chunk_info) {
    return chunk_info->data * 2;
}

size_t get_aligned(size_t value) {
    return 2 * (value / 2 + value % 2);
}
