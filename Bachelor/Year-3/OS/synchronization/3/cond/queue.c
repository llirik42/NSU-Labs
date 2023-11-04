#define _GNU_SOURCE

#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include "queue.h"

#define INIT_COND(COND) { \
    const int err = pthread_cond_init(COND, NULL); \
    if (err) {                    \
        printf("queue_init: pthread_cond_init(%s) failed: %s\n", #COND, strerror(err)); \
        abort();                  \
    }                         \
}

#define DESTROY_COND(COND) { \
    const int err = pthread_cond_destroy(COND); \
    if (err) {                    \
        printf("queue_destroy: pthread_cond_destroy(%s) failed: %s\n", #COND, strerror(err)); \
    }                         \
}

#define INIT_MUTEX(MUTEX) { \
    const int err = pthread_mutex_init(MUTEX, NULL); \
    if (err) {                    \
        printf("queue_init: pthread_mutex_init(%s) failed: %s\n", #MUTEX, strerror(err)); \
        abort();                  \
    }                         \
}

#define DESTROY_MUTEX(MUTEX) { \
    const int err = pthread_mutex_destroy(MUTEX); \
    if (err) {                    \
        printf("queue_destroy: pthread_mutex_destroy(%s) failed: %s\n", #MUTEX, strerror(err)); \
    }                         \
}

qnode_t *create_node(int val) {
    qnode_t *new = malloc(sizeof(qnode_t));
    if (!new) {
        printf("Cannot allocate memory for new node\n");
        abort();
    }
    new->val = val;
    new->next = NULL;

    return new;
}

void append(queue_t* q, qnode_t* qnode) {
    if (!q->first)
        q->first = q->last = qnode;
    else {
        q->last->next = qnode;
        q->last = q->last->next;
    }
    q->count++;
    q->add_count++;
    q->is_empty = false;
    q->is_full = q->count == q->max_count;
}

int pop(queue_t *q) {
    qnode_t *tmp = q->first;

    int ret = tmp->val;
    q->first = q->first->next;

    free(tmp);
    q->count--;
    q->get_count++;
    q->is_empty = q->count == 0;
    q->is_full = false;

    return ret;
}

void *qmonitor(void *arg) {
    queue_t *q = (queue_t *) arg;

    printf("qmonitor: [%d %d %d]\n", getpid(), getppid(), gettid());

    while (1) {
        queue_print_stats(q);
        usleep(1000);
    }
}

queue_t *queue_init(int max_count) {
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        printf("Cannot allocate memory for a queue\n");
        abort();
    }

    q->first = NULL;
    q->last = NULL;
    q->max_count = max_count;
    q->count = 0;

    q->add_attempts = q->get_attempts = 0;
    q->add_count = q->get_count = 0;

    q->is_empty = true;
    q->is_full = false;

    INIT_COND(&(q->cond_non_empty))
    INIT_COND(&(q->cond_non_full))
    INIT_MUTEX(&(q->mutex))

    const int err2 = pthread_create(&q->qmonitor_tid, NULL, qmonitor, q);
    if (err2) {
        printf("queue_init: pthread_create() failed: %s\n", strerror(err2));
        abort();
    }

    return q;
}

void queue_destroy(queue_t *q) {
    const int err1 = pthread_cancel(q->qmonitor_tid);
    if (err1) {
        printf("queue_destroy(): pthread_cancel() failed: %s\n", strerror(err1));
    }

    DESTROY_COND(&(q->cond_non_empty))
    DESTROY_COND(&(q->cond_non_full))
    DESTROY_MUTEX(&(q->mutex))

    qnode_t *cur = q->first;
    while (cur != NULL) {
        qnode_t *next = cur->next;
        free(cur);
        cur = next;
    }

    free(q);
}

int queue_add(queue_t *q, int val) {
    pthread_mutex_lock(&(q->mutex));
    while (q->is_full) {
        pthread_cond_wait(&(q->cond_non_full), &(q->mutex));
    }
    q->add_attempts++;
    assert(q->count <= q->max_count);
    qnode_t *new_node = create_node(val);
    append(q, new_node);
    pthread_cond_signal(&(q->cond_non_empty));
    pthread_mutex_unlock(&(q->mutex));

    return 1;
}

int queue_get(queue_t *q, int *val) {
    pthread_mutex_lock(&(q->mutex));
    while (q->is_empty) {
        pthread_cond_wait(&(q->cond_non_empty), &(q->mutex));
    }
    q->get_attempts++;
    assert(q->count >= 0);
    *val = pop(q);
    pthread_cond_signal(&(q->cond_non_full));
    pthread_mutex_unlock(&(q->mutex));

    return 1;
}

void queue_print_stats(queue_t *q) {
    pthread_mutex_lock(&(q->mutex));
    const int count = q->count;
    const long add_attempts = q->add_attempts;
    const long get_attempts = q->get_attempts;
    const long add_count = q->add_count;
    const long get_count = q->get_count;
    pthread_mutex_unlock(&(q->mutex));

    printf("queue stats: current size %d; attempts: (%ld %ld %ld); counts (%ld %ld %ld)\n",
           count,
           add_attempts, get_attempts, add_attempts - get_attempts,
           add_count, get_count, add_count - get_count);
}
