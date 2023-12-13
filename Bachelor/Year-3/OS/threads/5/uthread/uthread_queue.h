#pragma once

#include <stdint-gcc.h>
#include "uthread_data.h"

#define UTHREAD_QUEUE_SUCCESS 0
#define UTHREAD_QUEUE_NO_MEMORY 1

struct uthread_queue;

struct uthread_queue* uthread_queue_create();

void uthread_queue_destroy(struct uthread_queue* uthread_queue);

int uthread_queue_append(struct uthread_queue* uthread_queue, struct uthread_data* uthread_data);

struct uthread_data* uthread_queue_get(const struct uthread_queue* uthread_queue, unsigned int index);

void uthread_queue_remove(struct uthread_queue* uthread_queue, unsigned int tid);

unsigned int uthread_queue_length(const struct uthread_queue* uthread_queue);