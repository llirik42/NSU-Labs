#pragma once

#include "list.h"

int create_gc_thread(struct list* zombie_threads);

void notify_gc_thread();
