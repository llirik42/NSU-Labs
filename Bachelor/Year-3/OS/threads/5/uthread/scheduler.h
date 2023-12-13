#include <sys/time.h>
#include "uthread_data.h"

int scheduler_init(long schedule_time_usec);

void scheduler_shutdown();

int scheduler_add_uthread(struct uthread_data* uthread_data);

void scheduler_remove_uthread(struct uthread_data* uthread_data);
