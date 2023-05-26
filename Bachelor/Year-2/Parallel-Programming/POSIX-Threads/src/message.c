#include "message.h"

MPI_Datatype create_message_data_type(void) {
    const int block_lengths[] = {1, 1};
    const MPI_Aint offsets[] = {0, sizeof(int)};
    const MPI_Datatype fields_types[] = {MPI_INT, MPI_INT};

    MPI_Datatype ret;

    MPI_Type_create_struct(2,
                           block_lengths,
                           offsets,
                           fields_types,
                           &ret);

    MPI_Type_commit(&ret);

    return ret;
}
