#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <mpi.h>

#define TASKS_UPDATE_TYPE 0
#define PROCESS_RANK_REQUEST_TYPE 1
#define TERMINATION_TYPE 2

typedef struct {
    int type;
    int value;
} message_t;

MPI_Datatype create_message_data_type(void);
