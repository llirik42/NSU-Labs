#include <stdio.h>
#include <mpi/mpi.h>
#include "algorithm_data.h"

#define ROOT_RANK 0
#define DIMENSION_IN_GRID 2

void split_range(int range_size, int parts_count, int sizes[], int displs[]) {
    for (int i = 0; i < parts_count; i++) {
        const int k1 = i * range_size / parts_count;
        const int k2 = (i + 1) * range_size / parts_count;
        sizes[i] = k2 - k1;
        displs[i] = k1;
    }
}

double ddot(const double* vector1, const double* vector2, unsigned int size) {
    double result = 0;

    for (unsigned int i = 0; i < size; i++) {
        result += vector1[i] * vector2[i];
    }

    return result;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    const double start_time = MPI_Wtime();

    struct AlgorithmData algorithm_data = {
            .p1 = 0,
            .p2 = 0,
            .n1 = 0,
            .n2 = 0,
            .n3 = 0,
            .a = NULL,
            .b = NULL
    };

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == ROOT_RANK) {
        algorithm_data = get_algorithm_data();
    }

    // Broadcast input data from root to all
    MPI_Datatype mpi_input_data_type = get_algorithm_data_mpi_type();
    MPI_Bcast(&algorithm_data, 1, mpi_input_data_type, ROOT_RANK, MPI_COMM_WORLD);
    const int p1 = algorithm_data.p1;
    const int p2 = algorithm_data.p2;
    const int n1 = algorithm_data.n1;
    const int n2 = algorithm_data.n2;
    const int n3 = algorithm_data.n3;
    double* a = algorithm_data.a;
    double* b = algorithm_data.b;

    // Create communicator of grid
    MPI_Comm grid_comm;
    const int dims[DIMENSION_IN_GRID] = {p1, p2};
    const int periods[DIMENSION_IN_GRID] = {0, 0};
    MPI_Cart_create(MPI_COMM_WORLD,
                    DIMENSION_IN_GRID,
                    dims,
                    periods,
                    0,
                    &grid_comm);

    // Create communicator of row
    MPI_Comm row_comm;
    const int row_dims[DIMENSION_IN_GRID] = {0, 1};
    MPI_Cart_sub(grid_comm, row_dims, &row_comm);

    // Create communicator of column
    MPI_Comm column_comm;
    const int column_dims[DIMENSION_IN_GRID] = {1, 0};
    MPI_Cart_sub(grid_comm, column_dims, &column_comm);

    // Determine coordinates of process in grid
    int coords[2] = {0, 0}; // coordinates of process in grid (y, x)
    MPI_Cart_coords(grid_comm, rank, DIMENSION_IN_GRID, coords);
    const int y = coords[0];
    const int x = coords[1];

    // Split rows of matrix A between processes in grid
    int a_blocks_split[p1];
    int a_blocks_displs[p1];
    split_range(n1, p1, a_blocks_split, a_blocks_displs);

    // Determine rank of process in a column
    int column_rank;
    MPI_Comm_rank(column_comm, &column_rank);

    const int local_n1 = a_blocks_split[column_rank];
    double* local_a = allocate_matrix(local_n1, n2);

    // Split rows of A inside first column
    int a_elements_split[p1];
    int a_elements_displs[p1];
    if (x == 0) {
        for (unsigned int i = 0; i < p1; i++) {
            a_elements_split[i] = a_blocks_split[i] * n2;
            a_elements_displs[i] = a_blocks_displs[i] * n2;
        }

        MPI_Scatterv(a,
                     a_elements_split,
                     a_elements_displs,
                     MPI_DOUBLE,
                     local_a,
                     a_elements_split[column_rank],
                     MPI_DOUBLE,
                     ROOT_RANK,
                     column_comm);
    }

    // Propagate part of A inside row
    MPI_Bcast(local_a,
              local_n1 * n2,
              MPI_DOUBLE,
              ROOT_RANK,
              row_comm);

    // Split columns of matrix B between processes in grid
    int b_blocks_split[p2];
    int b_blocks_displs[p2];
    split_range(n3,p2, b_blocks_split, b_blocks_displs);

    // Determine rank of process in a row
    int row_rank;
    MPI_Comm_rank(row_comm, &row_rank);

    const int local_n3 = b_blocks_split[row_rank];
    double* local_b = allocate_matrix(n2, local_n3);

    MPI_Datatype column_type;
    MPI_Type_vector(n2, 1, n3, MPI_DOUBLE, &column_type);
    MPI_Type_commit(&column_type);

    MPI_Datatype resized_column_type;
    MPI_Type_create_resized(column_type, 0, sizeof(double), &resized_column_type);
    MPI_Type_commit(&resized_column_type);

    // Split columns of B inside first row
    if (y == 0) {
        MPI_Scatterv(b,
                     b_blocks_split,
                     b_blocks_displs,
                     resized_column_type,
                     local_b,
                     n2 * local_n3,
                     MPI_DOUBLE,
                     ROOT_RANK,
                     row_comm);
    }

    // Propagate part of B inside column
    MPI_Bcast(local_b,
              local_n3 * n2,
              MPI_DOUBLE,
              ROOT_RANK,
              column_comm);

    // Calculate local result
    double* local_result = allocate_matrix(local_n1, local_n3);
    for (unsigned int i = 0; i < local_n1; i++) {
        for (unsigned int j = 0; j < local_n3; j++) {
            local_result[i * local_n3 + j] = ddot(local_a + i * n2, local_b + j * n2, n2);
        }
    }

    // Gather result in root
    double* result = rank == ROOT_RANK ? allocate_matrix(n1, n3) : NULL;

    if (rank == ROOT_RANK) {
        for (unsigned int i = 0; i < local_n1; i++) {
            for (unsigned int j = 0; j < local_n3; j++) {
                result[i * n3 + j] = local_result[i * local_n3 + j];
            }
        }

        for (int i = 1; i < size; i++) {
            const int current_y = (i) / p2;
            const int current_x = (i) % p2;

            MPI_Datatype recv_type;
            MPI_Type_vector(a_blocks_split[current_y],
                            b_blocks_split[current_x],
                            n3,
                            MPI_DOUBLE,
                            &recv_type);
            MPI_Type_commit(&recv_type);

            MPI_Recv(result + a_blocks_displs[current_y] * n3 + b_blocks_displs[current_x],
                     1,
                     recv_type,
                     i,
                     i,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            MPI_Type_free(&recv_type);
        }
    }
    else {
        MPI_Send(local_result,
                 local_n1 * local_n3,
                 MPI_DOUBLE,
                 ROOT_RANK,
                 rank,
                 MPI_COMM_WORLD);
    }

    const double end_time = MPI_Wtime();

    if (rank == ROOT_RANK) {
        printf("Time: %.0f ms\n", (end_time - start_time) * 1000);
    }

    MPI_Type_free(&resized_column_type);
    MPI_Type_free(&column_type);
    MPI_Type_free(&mpi_input_data_type);

    destroy_algorithm_data(&algorithm_data);
    destroy(local_a);
    destroy(local_b);
    destroy(local_result);
    destroy(result);
    MPI_Finalize();
}
