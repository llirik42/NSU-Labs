#include "tasks_response.h"

MPI_Datatype create_tasks_response_data_type(MPI_Datatype task_data_type) {
    const int block_lengths[] = {1, 100};
    const MPI_Aint offsets[] = {0, sizeof(long)};
    const MPI_Datatype fields_types[] = {MPI_INT, task_data_type};

    MPI_Datatype ret;

    MPI_Type_create_struct(2,
                           block_lengths,
                           offsets,
                           fields_types,
                           &ret);

    MPI_Type_commit(&ret);

    return ret;
}
