#define _GNU_SOURCE

#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include "queue.h"

#define INIT_SEM(SEM, VAL) { \
    const int err = sem_init(SEM, 0, VAL); \
    if (err) {                    \
        printf("queue_init: sem_init(%s) failed: %s\n", #SEM, strerror(err)); \
        abort();                  \
    }                         \
}

#define DESTROY_SEM(SEM) { \
    const int err = sem_destroy(SEM); \
    if (err) {                    \
        printf("queue_destroy: sem_destroy(%s) failed: %s\n", #SEM, strerror(err)); \
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
}

int pop(queue_t *q) {
    qnode_t *tmp = q->first;

    int ret = tmp->val;
    q->first = q->first->next;

    free(tmp);
    q->count--;
    q->get_count++;

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

    INIT_SEM(&(q->sem_available), 1)
    INIT_SEM(&(q->sem_empty), 0)
    INIT_SEM(&(q->sem_full), max_count)

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

    DESTROY_SEM(&(q->sem_available))
    DESTROY_SEM(&(q->sem_empty))
    DESTROY_SEM(&(q->sem_full))

    qnode_t *cur = q->first;
    while (cur != NULL) {
        qnode_t *next = cur->next;
        free(cur);
        cur = next;
    }

    free(q);
}

int queue_add(queue_t *q, int val) {
    qnode_t *new_node = create_node(val);

    sem_wait(&(q->sem_full));
    sem_wait(&(q->sem_available));
    q->add_attempts++;
    assert(q->count <= q->max_count);
    append(q, new_node);
    sem_post(&(q->sem_available));
    sem_post(&(q->sem_empty));

    return 1;
}

int queue_get(queue_t *q, int *val) {
    sem_wait(&(q->sem_empty));

    sem_wait(&(q->sem_available));
    q->get_attempts++;
    assert(q->count >= 0);
    *val = pop(q);
    sem_post(&(q->sem_available));
    sem_post(&(q->sem_full));

    return 1;
}

void queue_print_stats(queue_t *q) {
    sem_wait(&(q->sem_available));
    const int count = q->count;
    const long add_attempts = q->add_attempts;
    const long get_attempts = q->get_attempts;
    const long add_count = q->add_count;
    const long get_count = q->get_count;
    sem_post(&(q->sem_available));

    printf("queue stats: current size %d; attempts: (%ld %ld %ld); counts (%ld %ld %ld)\n",
           count,
           add_attempts, get_attempts, add_attempts - get_attempts,
           add_count, get_count, add_count - get_count);
}
