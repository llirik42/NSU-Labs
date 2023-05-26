#include <math.h>
#include <stdlib.h>
#include "task.h"
#include "config.h"

task_t create_task(int iter_counter, int task_index, int rank, int size) {
    const task_t ret = {
            .repeat_number = abs(TASKS_IN_LIST_PER_PROCESS / 2 - task_index % TASKS_IN_LIST_PER_PROCESS) * abs(rank - (iter_counter % size)) * L
    };

    return ret;
}

double execute_task(const task_t* task) {
    double ret = 0;

    for(long i = 1; i <= task->repeat_number; i++) {
        ret += sin((double)i);
    }

    return ret;
}

MPI_Datatype create_task_data_type() {
    const int block_lengths[] = {1};
    const MPI_Aint offsets[] = {0};
    const MPI_Datatype fields_types[] = {MPI_UNSIGNED_LONG};

    MPI_Datatype ret;

    MPI_Type_create_struct(1,
                           block_lengths,
                           offsets,
                           fields_types,
                           &ret);

    MPI_Type_commit(&ret);

    return ret;
}
