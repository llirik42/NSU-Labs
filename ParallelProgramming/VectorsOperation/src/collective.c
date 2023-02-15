#include <mpi/mpi.h>

#include "config.h"
#include "utils.h"

int main(int argc, char** argv){
    int rank, size;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int vectors_size = 0;
    int elements_for_process = 0;

    double* local_vector1 = NULL;
    double* global_vector1 = NULL;
    double* vector2 = NULL;

    double global_result = 0;

    double start_time = 0;

    if (rank == ROOT_RANK) {
        vectors_size = (int) input_vectors_size();
        global_vector1 = create_vector(vectors_size);
        vector2 = create_vector(vectors_size);
        input_vectors(global_vector1, vector2);

        start_time = MPI_Wtime();
    }

    MPI_Bcast(&vectors_size,
              1,
              MPI_INT,
              ROOT_RANK,
              MPI_COMM_WORLD);

    local_vector1 = create_vector(vectors_size);

    if (rank != ROOT_RANK){
        vector2 = create_vector(vectors_size);
    }

    elements_for_process = vectors_size / size;

    MPI_Scatter(global_vector1,
                elements_for_process,
                MPI_DOUBLE,
                local_vector1,
                elements_for_process,
                MPI_DOUBLE,
                ROOT_RANK,
                MPI_COMM_WORLD);

    MPI_Bcast(vector2,
              (int) vectors_size,
              MPI_DOUBLE,
              ROOT_RANK,
              MPI_COMM_WORLD);

    double local_result = operation(local_vector1,
                                    vector2,
                                    elements_for_process,
                                    vectors_size);

    MPI_Reduce(&local_result,
               &global_result,
               1,
               MPI_DOUBLE,
               MPI_SUM,
               ROOT_RANK,
               MPI_COMM_WORLD);

    if (rank == ROOT_RANK){
        double end_time = MPI_Wtime();

        print_time(start_time * 1000, end_time * 1000);

        check_result(global_result);
    }

    MPI_Finalize();

    destroy_vector(local_vector1);
    destroy_vector(global_vector1);
    destroy_vector(vector2);

    return 0;
}
