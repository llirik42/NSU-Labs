#include <string.h>
#include "algorithm_data.h"
#include "config.h"

struct Matrix* get_a() {
    struct Matrix* a = allocate_matrix(N1, N2);

    for (size_t i = 0; i < N1; i++) {
        for (size_t j = 0; j < N2; j++) {
            set(a, i, j, A_IJ(i, j));
        }
    }

    return a;
}

struct Matrix* get_b() {
    struct Matrix* b = allocate_matrix(N2, N3);

    for (size_t i = 0; i < N2; i++) {
        for (size_t j = 0; j < N3; j++) {
            set(b, i, j, B_IJ(i, j));
        }
    }

    return b;
}

struct AlgorithmData get_algorithm_data() {
    struct Matrix* a = get_a();
    struct Matrix* b = get_b();

    if (a == NULL || b == NULL) {
        destroy_matrix(a);
        destroy_matrix(b);
        a = NULL;
        b = NULL;
    }

    struct AlgorithmData result = {
            .n1 = N1,
            .n2 = N2,
            .n3 = N3,
            .p1 = P1,
            .p2 = P2,
            .a = a,
            .b = b
    };

    return result;
}

void destroy_algorithm_data(struct AlgorithmData* algorithm_data) {
    destroy_matrix(algorithm_data->a);
    destroy_matrix(algorithm_data->b);
}

MPI_Datatype get_algorithm_data_mpi_type() {
    const int block_lengths[5] = {1,1, 1, 1, 1};

    const MPI_Aint size_of_int = sizeof(unsigned int);
    MPI_Aint offsets[5];
    for (unsigned int i = 0; i < 5; i++) {
        offsets[i] = i * size_of_int;
    }

    const MPI_Datatype fields_types[5] = {
            MPI_INT,
            MPI_INT,
            MPI_INT,
            MPI_INT,
            MPI_INT
    };
    MPI_Datatype mpi_input_data;

    MPI_Type_create_struct(5,
                           block_lengths,
                           offsets,
                           fields_types,
                           &mpi_input_data);

    MPI_Type_commit(&mpi_input_data);

    return mpi_input_data;
}
