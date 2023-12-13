#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include "utils.h"
#include "scheduler.h"
#include "uthread_queue.h"

struct uthread_queue* uthread_queue;
struct sigaction old_sigaction;

void schedule();

#define USECONDS_IN_SECOND 1000000

void schedule() {
    static unsigned int current_thread_id = 0;
    if (uthread_queue_length(uthread_queue) == 0) {
        return;
    }

    const unsigned int next_thread_id = (current_thread_id + 1) % uthread_queue_length(uthread_queue);

    struct uthread_data* current_thread = uthread_queue_get(uthread_queue, current_thread_id);
    struct uthread_data* next_thread = uthread_queue_get(uthread_queue, next_thread_id);
    current_thread_id = next_thread_id;

    block_sigalarm();
    swapcontext(&current_thread->ctx, &next_thread->ctx);
    unblock_sigalarm();
}

int create_fictive_main_uthread(struct uthread_data** main_uthread_data) {
    void* region_start;
    long region_size = get_page_size();

    if (create_mmap_region(&region_start, region_size) == -1) {
        return EAGAIN;
    }

    *main_uthread_data = region_start;
    (*main_uthread_data)->tid = -1;
    getcontext(&(*main_uthread_data)->ctx);

    return 0;
}

int scheduler_init(long schedule_time_usec) {
    uthread_queue = uthread_queue_create();

    if (uthread_queue == NULL) {
        return EAGAIN;
    }

    struct sigaction sa;
    sa.sa_handler = schedule;
    sa.sa_flags = SA_STACK;
    sigaction(SIGALRM, &sa, &old_sigaction);

    struct itimerval timer;
    timer.it_interval.tv_sec = schedule_time_usec / USECONDS_IN_SECOND;
    timer.it_interval.tv_usec = schedule_time_usec % USECONDS_IN_SECOND;
    timer.it_value = timer.it_interval;
    setitimer(ITIMER_REAL, &timer, NULL);

    struct uthread_data* main_uthread_data;
    if (create_fictive_main_uthread(&main_uthread_data)) {
        return EAGAIN;
    }

    uthread_queue_append(uthread_queue, main_uthread_data);

    return 0;
}

void scheduler_shutdown() {
    setitimer(ITIMER_REAL, NULL, NULL);
    uthread_queue_destroy(uthread_queue);
    sigaction(SIGALRM, &old_sigaction, NULL);
}

int scheduler_add_uthread(struct uthread_data* uthread_data) {
    return uthread_queue_append(uthread_queue, uthread_data) == UTHREAD_QUEUE_SUCCESS ? 0 : -1;
}

void scheduler_remove_uthread(struct uthread_data* uthread_data) {
    uthread_queue_remove(uthread_queue, uthread_data->tid);
    schedule();
}
