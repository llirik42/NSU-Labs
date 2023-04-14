#include <stdio.h>
#include <malloc.h>
#include <mpi/mpi.h>
#include "src/algorithm/algorithm_data.h"
#include "utils.h"

#define ERROR_CODE (-1)
#define ROOT_RANK 0

void print_matrix(const double* matrix, size_t rows, size_t columns) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < columns; j++) {
            printf("%-8.1f ", matrix[i * columns + j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void split_range(int range_size, int parts_count, int sizes[], int displs[]) {
    for (int i = 0; i < parts_count; i++) {
        const int k1 = i * range_size / parts_count;
        const int k2 = (i + 1) * range_size / parts_count;
        sizes[i] = k2 - k1;
        displs[i] = k1;
    }
}

double ddot(const double* v1, const double* v2, size_t length) {
    double result = 0;

    for (size_t i = 0; i < length; i++) {
        result += v1[i] * v2[i];
    }

    return result;
}


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    MPI_Datatype mpi_input_data;

    struct AlgorithmData input_data = {
            .n1 = 0,
            .n2 = 0,
            .n3 = 0,
            .p1 = 0,
            .p2 = 0,
            .a = NULL,
            .b = NULL
    };

    double* local_a = NULL;
    double* local_b = NULL;
    double* local_result = NULL;
    double* result = NULL;

    int comm_world_rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (comm_world_rank == ROOT_RANK) {
        input_data = get_algorithm_data();

        if (input_data.a == NULL || input_data.b == NULL) {
            fprintf(stderr, "Not enough memory for matrices\n");
            destroy_algorithm_data(&input_data);
            MPI_Abort(MPI_COMM_WORLD, ERROR_CODE);
        }

        result = allocate_matrix(input_data.n1, input_data.n3);
    }

    mpi_input_data = get_algorithm_data_mpi_type();
    MPI_Bcast(&input_data, 1, mpi_input_data, ROOT_RANK, MPI_COMM_WORLD);

    MPI_Comm grid_comm;
    const int dims[2] = {input_data.p1, input_data.p2};
    const int periods[2] = {0, 0};
    MPI_Cart_create(MPI_COMM_WORLD,
                    2,
                    dims,
                    periods,
                    0,
                    &grid_comm);

    MPI_Comm row_comm;
    const int row_cords[2] = {0, 1};
    MPI_Cart_sub(grid_comm, row_cords, &row_comm);

    MPI_Comm column_comm;
    const int column_coords[2] = {1, 0};
    MPI_Cart_sub(grid_comm, column_coords, &column_comm);

    int coords[2] = {0, 0};
    int grid_rank;
    MPI_Comm_rank(grid_comm, &grid_rank);
    MPI_Cart_coords(grid_comm, grid_rank, 2, coords);

    int sends_count[input_data.p1];
    int displs[input_data.p1];

    int multiplied_sends_count[input_data.p1];
    int multiplied_displs[input_data.p1];

    split_range(input_data.n1, input_data.p1, sends_count, displs);

    for (unsigned int i = 0; i < input_data.p1; i++) {
        multiplied_sends_count[i] = input_data.n2 * sends_count[i];
        multiplied_displs[i] = input_data.n2 * displs[i];
    }

    int column_rank;
    MPI_Comm_rank(column_comm, &column_rank);

    int row_rank;
    MPI_Comm_rank(row_comm, &row_rank);

    int local_n1 = multiplied_sends_count[column_rank];

    local_a = allocate_matrix(local_n1, input_data.n2);

    if (coords[1] == 0) {
        MPI_Scatterv(input_data.a,
                     multiplied_sends_count,
                     multiplied_displs,
                     MPI_DOUBLE,
                     local_a,
                     input_data.n1 * input_data.n2,
                     MPI_DOUBLE,
                     ROOT_RANK,
                     column_comm);
    }

    MPI_Bcast(local_a, local_n1 * input_data.n2, MPI_DOUBLE, 0, row_comm);

    int columns_sizes[input_data.p2];
    int columns_displs[input_data.p2];
    split_range(input_data.n3, input_data.p2, columns_sizes, columns_displs);
    int local_n3 = columns_sizes[row_rank];

    local_b = allocate_matrix(input_data.n2, local_n3);

    MPI_Datatype column_type;
    MPI_Type_vector(input_data.n2, 1, input_data.n3, MPI_DOUBLE, &column_type);
    MPI_Type_commit(&column_type);

    MPI_Datatype resized_column;
    MPI_Type_create_resized(column_type, 0, sizeof(double), &resized_column);
    MPI_Type_commit(&resized_column);

    if (coords[0] == 0) {
        MPI_Scatterv(input_data.b,
                     columns_sizes,
                     columns_displs,
                     resized_column,
                     local_b,
                     input_data.n2 * local_n3,
                     MPI_DOUBLE,
                     0, row_comm);
    }

    MPI_Bcast(local_b, local_n3 * input_data.n2, MPI_DOUBLE, 0, column_comm);

    local_result = allocate_matrix(local_n1, local_n3);

    for (unsigned int i = 0; i < local_n1; i++) {
        for (unsigned int j = 0; j < local_n3; j++) {
            local_result[i * local_n3 + j] = ddot(local_a + input_data.n2 * i, local_b + input_data.n2 * j, input_data.n2);
        }
    }

    MPI_Datatype send_block;
    MPI_Type_contiguous(local_n1 * local_n3, MPI_DOUBLE, &send_block);
    MPI_Type_commit(&send_block);

//    MPI_Datatype resized_recv_block;
//    MPI_Type_create_resized(recv_block, 0, (int) (sizeof(double)) * local_n3, &resized_recv_block);
//    MPI_Type_commit(&resized_recv_block);

//    if (grid_rank == ROOT_RANK) {
//        for (int i = 0; i < input_data.p1; i++) {
//            for (int j = 0; j < input_data.p2; j++) {
//                if (j == 0 && i == 0) {
//                    final_displs[0] = 0;
//                    final_sends_count[0] = columns_sizes[0] * sends_count[0];
//                    //printf("sends_count[%d] = %d, displs[%d] = %d\n", 0, final_sends_count[0], 0, final_displs[0]);
//                    continue;
//                }
//
//                const int index = j + i * input_data.p2;
//
//                final_sends_count[index] = columns_sizes[j] * sends_count[i];
//                final_displs[index] = i * input_data.n3 * sends_count[i] + columns_displs[j];
//
//                //printf("sends_count[%d] = %d, displs[%d] = %d\n", index, final_sends_count[index], index, final_displs[index]);
//            }
//        }
//    }

    if (grid_rank != 0) {
        MPI_Send(local_result,
                 1,
                 send_block,
                 ROOT_RANK,
                 grid_rank,
                 MPI_COMM_WORLD);

    }
    else {
        MPI_Status status;

        for (int i = 1; i < size; i++) {
            MPI_Datatype recv_block;
            MPI_Type_vector(sends_count[i], columns_sizes[i], input_data.n3, MPI_DOUBLE, &recv_block);
            MPI_Type_commit(&recv_block);

            const int offset = 2 * (i % 4) + (i / 4) * 32;
            MPI_Recv(result + offset,
                     1,
                     recv_block,
                     i,
                     i,
                     MPI_COMM_WORLD,
                     &status);

            MPI_Type_free(&recv_block);
        }
    }




//    MPI_Gatherv(local_result,
//                1,
//                send_block,
//                result,
//                final_sends_count,
//                final_displs,
//                resized_recv_block,
//                ROOT_RANK,
//                MPI_COMM_WORLD);

    if (grid_rank == ROOT_RANK) {
        print_matrix(result, input_data.n1, input_data.n3);
    }

    free(local_a);
    free(local_b);
    free(local_result);
    free(result);
    MPI_Type_free(&mpi_input_data);
    MPI_Type_free(&resized_column);
    MPI_Type_free(&column_type);
    MPI_Type_free(&send_block);
    //MPI_Type_free(&resized_recv_block);
    destroy_algorithm_data(&input_data);
    MPI_Finalize();
}
