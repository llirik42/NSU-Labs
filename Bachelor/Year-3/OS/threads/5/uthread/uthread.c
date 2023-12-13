#include <errno.h>
#include <string.h>
#include "scheduler.h"
#include "uthread_data.h"
#include "utils.h"
#include "uthread.h"

#define PAGES_IN_THREAD_STACK 20

void* thread_routine_wrapper(void* arg) {
    struct uthread_data* uthread_data = arg;
    uthread_data->retval = uthread_data->routine(uthread_data->arg);
    uthread_data->exited = YES;

    while (1) {}
}

int uthread_create(uthread_t* uthread, void* (* start_routine)(void*), void* arg) {
    static int new_thread_id = 0;

    const long stack_size = get_page_size() * PAGES_IN_THREAD_STACK;

    void* stack;
    if (create_mmap_region(&stack, stack_size) == -1) {
        return EAGAIN;
    }

    *uthread = stack + stack_size - sizeof(struct uthread_data);
    (*uthread)->tid = new_thread_id;
    (*uthread)->stack = stack;
    (*uthread)->stack_size = stack_size;
    (*uthread)->routine = start_routine;

    getcontext(&(*uthread)->ctx);
    (*uthread)->ctx.uc_stack.ss_sp = stack;
    (*uthread)->ctx.uc_stack.ss_size = stack_size - sizeof(struct uthread_data);
    (*uthread)->ctx.uc_link = NULL;
    (*uthread)->arg = arg;
    (*uthread)->exited = NO;

    makecontext(&(*uthread)->ctx, (void (*)(void)) thread_routine_wrapper, 1, *uthread);
    new_thread_id++;

    block_sigalarm();
    const int code = scheduler_add_uthread(*uthread);
    unblock_sigalarm();

    if (code == -1) {
        uthread_data_free(*uthread);
        return EAGAIN;
    }

    return 0;
}

int uthread_join(uthread_t uthread, void** retval) {
    wait_on_yes_no_address(&(uthread->exited), YES);
    uthread->joined = YES;
    notify_all_on_address(&(uthread->joined));
    scheduler_remove_uthread(uthread);

    if (retval != NULL) {
        *retval = uthread->retval;
    }

    block_sigalarm();
    uthread_data_free(uthread);
    unblock_sigalarm();

    return 0;
}
