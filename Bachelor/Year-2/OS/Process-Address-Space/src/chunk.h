#pragma once

#include <aio.h>
#include <stdbool.h>

struct ChunkInfo;

void create_chunk_at(void* ptr, size_t length);

size_t get_aligned(size_t value);

size_t get_min_chunk_length();

size_t get_chunk_info_size();

size_t get_prev_chunk_length(const struct ChunkInfo* chunk_ptr);

size_t get_chunk_length(const struct ChunkInfo* chunk_ptr);

bool is_prev_chunk_free(const struct ChunkInfo* chunk_ptr);

bool is_chunk_free(const struct ChunkInfo* chunk_ptr);

bool is_next_chunk_free(const struct ChunkInfo* chunk_ptr);

bool is_chunk_valid(const struct ChunkInfo* chunk_ptr);

void set_chunk_free(struct ChunkInfo* chunk_ptr);

void set_chunk_occupied(struct ChunkInfo* chunk_ptr);

void set_prev_chunk_length(struct ChunkInfo* chunk_ptr, size_t length);

void set_chunk_length(struct ChunkInfo* chunk_ptr, size_t length);

void unite_chunk_with_next(struct ChunkInfo* chunk_ptr);
