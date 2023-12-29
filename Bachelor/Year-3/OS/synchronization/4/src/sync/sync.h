#pragma once

void* create_sync();

void destroy_sync(void* sync);

int rlock_sync(void* sync);

int wlock_sync(void* sync);

int unlock_sync(void* sync);
