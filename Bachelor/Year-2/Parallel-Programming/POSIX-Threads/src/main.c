#include <mpi.h>
#include "worker.h"
#include "manager.h"

int main(int argc, char** argv) {
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int workers_size = size - 1;
    const int manager_rank = workers_size;

    MPI_Group global_group;
    MPI_Comm_group(MPI_COMM_WORLD, &global_group);

    MPI_Group workers_group;
    int ranks[] = {manager_rank};
    MPI_Group_excl(global_group, 1, ranks, &workers_group);

    MPI_Comm workers_comm;
    MPI_Comm_create(MPI_COMM_WORLD, workers_group, &workers_comm);

    if (rank == size - 1) {
        manage(workers_size);
    } else {
        work(workers_comm, manager_rank);
    }

    MPI_Finalize();
}
