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

    if (rank == ROOT_RANK){
        vectors_size = (int) input_vectors_size();
        double* global_vector1 = create_vector(vectors_size);
        double* global_vector2 = create_vector(vectors_size);
        input_vectors(global_vector1, global_vector2);

        elements_for_process = vectors_size / size;

        double start_time = MPI_Wtime();

        for (int i = 1; i < size; i++){
            MPI_Send(&vectors_size,
                     1,
                     MPI_INT,
                     i,
                     0,
                     MPI_COMM_WORLD);

            MPI_Send(global_vector1 + elements_for_process * i,
                     elements_for_process,
                     MPI_DOUBLE,
                     i,
                     1,
                     MPI_COMM_WORLD);

            MPI_Send(global_vector2,
                     (int) vectors_size,
                     MPI_DOUBLE,
                     i,
                     2,
                     MPI_COMM_WORLD);
        }

        double global_result = operation(global_vector1,
                                   global_vector2,
                                   elements_for_process,
                                   vectors_size);

        for (int i = 1; i < size; i++){
            double partial_result;

            MPI_Recv(&partial_result,
                     1,
                     MPI_DOUBLE,
                     i,
                     i,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            global_result += partial_result;
        }

        double end_time = MPI_Wtime();

        print_time(start_time * 1000, end_time * 1000);

        check_result(global_result);

        destroy_vector(global_vector1);
        destroy_vector(global_vector2);
    }
    else{
        MPI_Recv(&vectors_size,
                 1,
                 MPI_INT,
                 ROOT_RANK,
                 0,
                 MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        elements_for_process = vectors_size / size;

        double* local_vector1 = create_vector(elements_for_process);
        double* local_vector2 = create_vector(vectors_size);

        MPI_Recv(local_vector1,
                 elements_for_process,
                 MPI_DOUBLE,
                 ROOT_RANK,
                 1,
                 MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        MPI_Recv(local_vector2,
                 (int) vectors_size,
                 MPI_DOUBLE,
                 ROOT_RANK,
                 2,
                 MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        double local_result = operation(local_vector1,
                                        local_vector2,
                                        elements_for_process,
                                        vectors_size);

        MPI_Send(&local_result,
                 1,
                 MPI_DOUBLE,
                 ROOT_RANK,
                 rank,
                 MPI_COMM_WORLD);

        destroy_vector(local_vector1);
        destroy_vector(local_vector2);
    }

    MPI_Finalize();

    return 0;
}
