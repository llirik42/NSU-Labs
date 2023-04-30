#pragma once

#include <aio.h>
#include <stdbool.h>

#define MIN_CHUNK_LENGTH 2

struct ChunkInfo {
    size_t data: 63;
    size_t is_free: 1;
};

struct ChunkInfo create_chunk_info(size_t length);

void create_chunk_info_at(void* ptr, size_t length);

void set_chunk_free(struct ChunkInfo* chunk_info);

void set_chunk_occupied(struct ChunkInfo* chunk_info);

void set_chunk_length(struct ChunkInfo* chunk_info, size_t length);

bool is_chunk_occupied(const struct ChunkInfo* chunk_info);

size_t get_chunk_length(const struct ChunkInfo* chunk_info);

size_t get_aligned(size_t value);
