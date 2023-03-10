#include <mpi/mpi.h>
#include <malloc.h>
#include "utils.h"

#define ROOT_RANK 0

struct IterationsData {
    double* local_A;
    double* local_b;
    double* x0;
    double epsilon;
    unsigned int Nx;
    unsigned int Ny;
    unsigned int max_iterations_count;
    int* sends_count; 
    int* displs;
};

unsigned int iterate(const struct IterationsData* iterations_data, double* local_result) {
    const double* local_A = iterations_data->local_A;
    const double* local_b = iterations_data->local_b;
    const double* x0 = iterations_data->x0;
    const double epsilon = iterations_data->epsilon;
    const unsigned int Nx = iterations_data->Nx;
    const unsigned int Ny = iterations_data->Ny;
    const unsigned int max_iterations_count = iterations_data->max_iterations_count;
    const int* sends_count = iterations_data->sends_count;
    const int* displs = iterations_data->displs;

    unsigned int iterations_count = 0;

    double* global_z = create_vector(Nx);
    double* local_r = create_vector(Ny);
    double* local_z = create_vector(Ny);
    double* local_A_z = create_vector(Ny);
    double* buffer = create_vector(Ny);

    // Calculating |b|^2 * epsilon^2
    const double local_b_nrm_squared = ddot(local_b, local_b, Ny);
    double b_nrm_squared;
    MPI_Allreduce(&local_b_nrm_squared,
                  &b_nrm_squared,
                  1,
                  MPI_DOUBLE,
                  MPI_SUM,
                  MPI_COMM_WORLD);
    const double b_nrm_epsilon_squared = b_nrm_squared * epsilon * epsilon;

    // Calculating r_0 and z_0
    multiply_vector_by_matrix(local_A, x0, Nx, Ny, local_r);
    inverse_subtract_vectors(local_r, local_b, Ny);
    copy_vector(local_r, local_z, Ny);

    // Calculating (r_0, r_0)
    double local_r_nrm_squared = ddot(local_r, local_r, Ny);
    double r_nrm_squared;
    MPI_Allreduce(&local_r_nrm_squared,
                  &r_nrm_squared,
                  1,
                  MPI_DOUBLE,
                  MPI_SUM,
                  MPI_COMM_WORLD); 

    // Iterative cycle 
    while(1) {
        // Checking condition of exit 
        if (r_nrm_squared < b_nrm_epsilon_squared || iterations_count > max_iterations_count) {
            break;
        }

        // Gathering the whole z_n
        MPI_Allgatherv(local_z,
                       Ny,
                       MPI_DOUBLE,
                       global_z,
                       sends_count,
                       displs,
                       MPI_DOUBLE,
                       MPI_COMM_WORLD);

        // Calculating A x z_n
        multiply_vector_by_matrix(local_A, global_z, Nx, Ny, local_A_z);

        // Calculating alpha
        const double alpha_numerator = r_nrm_squared;
        const double local_alpha_denominator = ddot(local_A_z, local_z, Ny);
        double alpha_denominator;        
        MPI_Allreduce(&local_alpha_denominator,
                      &alpha_denominator,
                      1,
                      MPI_DOUBLE,
                      MPI_SUM,
                      MPI_COMM_WORLD);
        const double alpha = alpha_numerator / alpha_denominator;

        // Calculating x_{n+1}
        multiply_vector_by_scalar(local_z, alpha, Ny, buffer);
        add_vectors(local_result, buffer, Ny);

        // Calculating r_{n+1}
        multiply_vector_by_scalar(local_A_z, alpha, Ny, buffer);
        subtract_vectors(local_r, buffer, Ny);

        // Calculating beta
        const double beta_denominator = r_nrm_squared;
        const double local_r_next_nrm_squared = ddot(local_r, local_r, Ny);
        MPI_Allreduce(&local_r_next_nrm_squared,
                      &r_nrm_squared,
                      1,
                      MPI_DOUBLE,
                      MPI_SUM,
                      MPI_COMM_WORLD);
        const double beta = r_nrm_squared / beta_denominator;

        // Calculating z_{n+1}
        multiply_vector_by_scalar(local_z, beta, Ny, local_z);
        add_vectors(local_z, local_r, Ny);

        iterations_count++;
    }

    free(global_z);
    free(local_r);
    free(local_z);
    free(local_A_z);
    free(buffer);

    return iterations_count;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    const double start_time = MPI_Wtime();

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double* A = NULL;
    double* b = NULL;
    double* x0 = NULL;
    double epsilon;
    unsigned int Nx;
    unsigned int Ny;
    unsigned int max_iterations_count;

    int* vector_sends_count = create_int_vector(size);
    int* vector_displs = create_int_vector(size);
    int* matrix_sends_count = NULL;
    int* matrix_displs = NULL;

    double* result = NULL;
    
    if (rank == ROOT_RANK) {
        const struct InputData input_data = create_input_data();

        A = input_data.A;
        b = input_data.b;
        x0 = input_data.x0;
        epsilon = input_data.epsilon;
        Nx = input_data.n;
        max_iterations_count = input_data.max_iterations_count;

        matrix_sends_count = create_int_vector(size);
        matrix_displs = create_int_vector(size);

        result = create_vector(Nx);

        for (int i = 0; i < size; i++) {
            const int k1 = i * Nx / size;
            const int k2 = (i + 1) * Nx / size;

            vector_sends_count[i] = k2 - k1;
            matrix_sends_count[i] = vector_sends_count[i] * Nx;
            vector_displs[i] = k1;    
            matrix_displs[i] = vector_displs[i] * Nx;        
        }
    }

    MPI_Bcast(&max_iterations_count, 
              1, 
              MPI_UNSIGNED, 
              ROOT_RANK, 
              MPI_COMM_WORLD);

    MPI_Bcast(&epsilon, 
              1, 
              MPI_DOUBLE, 
              ROOT_RANK, 
              MPI_COMM_WORLD);

    MPI_Bcast(vector_sends_count, 
              size, 
              MPI_INT, 
              ROOT_RANK, 
              MPI_COMM_WORLD);

    MPI_Bcast(vector_displs, 
              size, 
              MPI_INT, 
              ROOT_RANK, 
              MPI_COMM_WORLD);

    // Calculating Nx, Ny
    Ny = vector_sends_count[rank];
    Nx = 0;
    for (unsigned int i = 0; i < size; i++) {
        Nx += vector_sends_count[i];
    }

    double* local_A = create_matrix(Nx, Ny);
    double* local_b = create_vector(Ny);
    double* local_result = create_vector(Ny);
    if (rank != ROOT_RANK) {
        x0 = create_vector(Nx);
    }

    MPI_Bcast(x0, 
              Nx, 
              MPI_DOUBLE, 
              ROOT_RANK, 
              MPI_COMM_WORLD);

    MPI_Scatterv(A, 
                 matrix_sends_count,
                 matrix_displs,
                 MPI_DOUBLE,
                 local_A,
                 Nx * Ny,
                 MPI_DOUBLE,
                 ROOT_RANK,
                 MPI_COMM_WORLD);

    MPI_Scatterv(b, 
                 vector_sends_count,
                 vector_displs,
                 MPI_DOUBLE,
                 local_b,
                 Ny,
                 MPI_DOUBLE,
                 ROOT_RANK,
                 MPI_COMM_WORLD);

    // Setting local_x0
    const unsigned int offset = vector_displs[rank];
    for (unsigned int i = 0; i < Ny; i++) {
        local_result[i] = x0[i + offset];
    }
    
    const struct IterationsData iterarations_data = {
        .local_A = local_A,
        .local_b = local_b,
        .x0 = x0,
        .epsilon = epsilon,
        .Nx = Nx,
        .Ny = Ny,
        .max_iterations_count = max_iterations_count,
        .sends_count = vector_sends_count,
        .displs = vector_displs
    };

    const unsigned int iterations_count = iterate(&iterarations_data, local_result);

    // Gather final result
    MPI_Gatherv(local_result, 
                Ny,
                MPI_DOUBLE,
                result,
                vector_sends_count,
                vector_displs,
                MPI_DOUBLE,
                ROOT_RANK,
                MPI_COMM_WORLD);

    if (rank == ROOT_RANK) {
        const double end_time = MPI_Wtime();
        print_result_info(start_time * 1000,
                          end_time * 1000,
                          iterations_count,
                          A,
                          b,
                          result);
    }

    free(A);
    free(b);
    free(x0);
    free(vector_sends_count);
    free(matrix_sends_count);
    free(vector_displs);
    free(matrix_displs);
    free(local_A);
    free(local_b);
    free(local_result);
    free(result);

    MPI_Finalize();

    return 0;
}
