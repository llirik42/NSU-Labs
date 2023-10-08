#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>
#include "gc_thread.h"
#include "mythread.h"
#include "utils.h"
#include "list.h"
#include <errno.h>

#define PAGES_IN_THREAD_STACK 20
#define MAIN_THREAD_ID 0

#define MAIN_THREAD_MMAP_PAGES_SIZE 1

static int new_thread_id = 1;
unsigned int executing_threads_count = 0; // Except gc
struct list* alive_threads = NULL;
struct list* zombie_threads = NULL;

bool is_main_mythread(mythread_t mythread) {
    return mythread->id == MAIN_THREAD_ID;
}
int create_main_mythread(mythread_t* mythread) {
    const long region_size = get_page_size() * MAIN_THREAD_MMAP_PAGES_SIZE;
    void* region;
    if (create_mmap_region(&region, region_size) == -1) {
        return -1;
    }

    *mythread = region;
    (*mythread)->id = MAIN_THREAD_ID;
    (*mythread)->pid = getpid();
    (*mythread)->stack = region;
    (*mythread)->stack_size = region_size;
    (*mythread)->start_routine = NULL;
    (*mythread)->arg = NULL;
    (*mythread)->retval = NULL;
    (*mythread)->exited = NO;
    (*mythread)->joined = NO;
    (*mythread)->detached = NO;

    return 0;
}

void __attribute__ ((constructor)) library_startup() {
    alive_threads = create_list();
    zombie_threads = create_list();

    if (alive_threads == NULL || zombie_threads == NULL) {
        fprintf(stderr, "Not enough resources for \"<mythread.h>\"\n");
        exit(1);
    }

    if (create_gc_thread(zombie_threads) == -1) {
        fprintf(stderr, "Not enough resources for \"<mythread.h>\"\n");
        exit(1);
    }

    mythread_t main_mythread;
    if (create_main_mythread(&main_mythread) == -1) {
        fprintf(stderr, "Not enough resources for \"<mythread.h>\"\n");
        exit(1);
    }

    executing_threads_count++;
    append(alive_threads, main_mythread);
}

int startup(void* arg) {
    mythread_t mythread = arg;

    mythread->retval = mythread->start_routine(mythread->arg);
    mythread->exited = YES;

    executing_threads_count--;
    notify_all_on_address(&(mythread->exited));

    if (!mythread->detached && executing_threads_count != 0) {
        wait_on_yes_no_address(&(mythread->joined), YES);
    }

    mythread_exit(mythread->retval);

    return SUCCESS;
}

int mythread_create(mythread_t* mythread, void* (*start_routine)(void*), void* arg) {
    const long stack_size = get_page_size() * PAGES_IN_THREAD_STACK;

    void* stack;
    if (create_mmap_region(&stack, stack_size) == -1) {
        return EAGAIN;
    }

    *mythread = stack + stack_size - sizeof(struct mythread_data_t);
    (*mythread)->id = new_thread_id;
    (*mythread)->stack = stack;
    (*mythread)->stack_size = stack_size;
    (*mythread)->start_routine = start_routine;
    (*mythread)->arg = arg;
    (*mythread)->retval = NULL;
    (*mythread)->exited = NO;
    (*mythread)->joined = NO;
    (*mythread)->detached = NO;

    new_thread_id++;
    executing_threads_count++;
    append(alive_threads, *mythread);

    (*mythread)->pid = create_thread(startup, (*mythread) - 1,  *mythread);
    if ((*mythread)->pid == -1) {
        int error = errno;
        free_mmap_region(stack, stack_size);
        return error;
    }

    return SUCCESS;
}

int mythread_join(mythread_t mythread, void** retval) {
    if (mythread->detached) {
        return EINVAL;
    }

    wait_on_yes_no_address(&(mythread->exited), YES);
    mythread->joined = YES;
    notify_all_on_address(&(mythread->joined));

    if (retval != NULL) {
        *retval = mythread->retval;
    }

    return SUCCESS;
}

int mythread_detach(mythread_t mythread) {
    if (!contains(alive_threads, mythread)) {
        return ESRCH;
    }

    mythread->detached = YES;

    return SUCCESS;
}

bool mythread_equal(mythread_t mythread1, mythread_t mythread2) {
    return mythread1->id == mythread2->id;
}

mythread_t mythread_self() {
    return find_by_pid(alive_threads, gettid());
}

void mythread_exit(void* retval) {
    if (executing_threads_count == 0) {
        exit(0);
    }
    else {
        mythread_t mythread = mythread_self();
        if (is_main_mythread(mythread)) {
            executing_threads_count--;
        }

        mythread->retval = retval;
        mythread->exited = YES;

        notify_all_on_address(&(mythread->exited));

        append(zombie_threads, mythread);
        delete_by_pid(alive_threads, mythread->pid);
        notify_gc_thread();
        syscall(SYS_exit, 0);
    }
}
