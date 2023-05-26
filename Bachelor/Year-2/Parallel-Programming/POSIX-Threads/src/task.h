#pragma once

#include <stddef.h>
#include <mpi.h>
#include <stdbool.h>

typedef struct {
    long repeat_number;
} task_t;

task_t create_task(int iter_counter, int task_index, int rank, int size);

double execute_task(const task_t* task);

MPI_Datatype create_task_data_type();
