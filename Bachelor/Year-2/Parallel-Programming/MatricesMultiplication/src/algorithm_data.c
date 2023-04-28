#include "algorithm_data.h"
#include "config.h"

#define FIELDS_IN_STRUCTURE ((sizeof(struct AlgorithmData) - sizeof(double*) * 2) / sizeof(int))

double* get_a() {
    double* a = allocate_matrix(N1, N2);

    for (unsigned int i = 0; i < N1; i++) {
        for (unsigned int j = 0; j < N2; j++) {
            set(a, N2, i, j, A_IJ(i, j));
        }
    }

    return a;
}

double* get_b() {
    double* b = allocate_matrix(N2, N3);

    for (unsigned int i = 0; i < N2; i++) {
        for (unsigned int j = 0; j < N3; j++) {
            set(b, N3, i, j, B_IJ(i, j));
        }
    }

    return b;
}

struct AlgorithmData get_algorithm_data() {
    double* a = get_a();
    double* b = get_b();

    struct AlgorithmData result = {
            .p1 = P1,
            .p2 = P2,
            .n1 = N1,
            .n2 = N2,
            .n3 = N3,
            .a = a,
            .b = b
    };

    return result;
}

void destroy_algorithm_data(struct AlgorithmData* algorithm_data) {
    destroy(algorithm_data->a);
    destroy(algorithm_data->b);
}

MPI_Datatype get_algorithm_data_mpi_type() {
    int block_lengths[FIELDS_IN_STRUCTURE];
    for (unsigned int i = 0; i < FIELDS_IN_STRUCTURE; i++) {
        block_lengths[i] = 1;
    }

    const MPI_Aint size_of_int = sizeof(unsigned int);
    MPI_Aint offsets[FIELDS_IN_STRUCTURE];
    for (unsigned int i = 0; i < FIELDS_IN_STRUCTURE; i++) {
        offsets[i] = i * size_of_int;
    }

    MPI_Datatype fields_types[FIELDS_IN_STRUCTURE];
    for (unsigned int i = 0; i < FIELDS_IN_STRUCTURE; i++) {
        fields_types[i] = MPI_INT;
    }

    MPI_Datatype mpi_input_data;

    MPI_Type_create_struct(FIELDS_IN_STRUCTURE,
                           block_lengths,
                           offsets,
                           fields_types,
                           &mpi_input_data);

    MPI_Type_commit(&mpi_input_data);

    return mpi_input_data;
}
