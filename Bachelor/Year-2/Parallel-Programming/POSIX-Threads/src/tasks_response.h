#pragma once

#include <mpi.h>
#include "task.h"

#define TASKS_RESPONSE_LENGTH 100

typedef struct {
    long tasks_count;
    task_t tasks[TASKS_RESPONSE_LENGTH];
} tasks_response_t;

MPI_Datatype create_tasks_response_data_type(MPI_Datatype task_data_type);
