#include <stdio.h>
#include <mpi/mpi.h>
#include <malloc.h>
#include <stdbool.h>

#include "config.h"
#include "utils.h"

void print_vector(double* vector, unsigned int length) {
    for (unsigned int i = 0; i < length; i++) {
        printf("%f ", vector[i]);
    }
    printf("\n");
}

int main(int argc, char** argv) {
	int rank, size;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    unsigned int Nx;
    unsigned int Ny;

    int* vector_sends_count = NULL;
    int* matrix_sends_count = NULL;
    int* vector_displs = NULL;
    int* matrix_displs = NULL;
    
    double* A = NULL;
    double* b = NULL;
    double* x0 = NULL;
    double epsilon = 0;


    // double* x_current = NULL;
    // double* x_next = NULL;
    // double* r_current = NULL;
    // double* r_next = NULL;
    // double* z_current = NULL;
    // double* z_next = NULL;

    if (rank == ROOT_RANK) {
        const struct InputData input_data = create_input_data();

        A = input_data.A;
        b = input_data.b;
        x0 = input_data.x0;
        epsilon = input_data.epsilon;
        const unsigned int N = input_data.N;

        vector_sends_count = malloc(size * sizeof(int));
        matrix_sends_count = malloc(size * sizeof(int));
        vector_displs = malloc(size * sizeof(int));
        matrix_displs = malloc(size * sizeof(int));

        // x_current = create_vector(N);
        // x_next = create_vector(N);
        // r_current = create_vector(N);
        // r_next = create_vector(N);
        // z_current = create_vector(N);
        // z_next = create_vector(N);

        //copy_vector(x_current, x0, N);

        for (int i = 0; i < size; i++) {
            int k1 = i * N / size;
            int k2 = (i + 1) * N / size;

            vector_sends_count[i] = k2 - k1;
            matrix_sends_count[i] = vector_sends_count[i] * N;
            vector_displs[i] = k1;    
            matrix_displs[i] = vector_displs[i] * N;
        }

        Nx = N;
    }
    
    MPI_Bcast(&Nx, 
              1, 
              MPI_UNSIGNED, 
              ROOT_RANK, 
              MPI_COMM_WORLD);

    MPI_Bcast(&epsilon, 
              1, 
              MPI_DOUBLE, 
              ROOT_RANK, 
              MPI_COMM_WORLD);

    MPI_Scatter(vector_sends_count,
                1,
                MPI_INT,
                &Ny,
                1,
                MPI_UNSIGNED,
                ROOT_RANK,
                MPI_COMM_WORLD);

    double* local_A = create_matrix(Nx, Ny);
    double* local_b = create_vector(Ny);
    double* local_x0 = create_vector(Ny);

    if (rank != ROOT_RANK) {
        x0 = create_vector(Nx);
        //z_current = create_vector(Nx);
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

    MPI_Scatterv(x0, 
                 vector_sends_count,
                 vector_displs,
                 MPI_DOUBLE,
                 local_x0,
                 Ny,
                 MPI_DOUBLE,
                 ROOT_RANK,
                 MPI_COMM_WORLD);

    double b_nrm_squared;
    double local_b_nrm_squared = calculate_nrm_squared(local_b, Ny);

    MPI_Allreduce(&local_b_nrm_squared,
                  &b_nrm_squared,
                  1,
                  MPI_DOUBLE,
                  MPI_SUM,
                  MPI_COMM_WORLD);

    double epislon_b_nrm_squared = b_nrm_squared * epsilon * epsilon;

    MPI_Barrier(MPI_COMM_WORLD);

    printf("%.20f\n", epislon_b_nrm_squared);



    // double* local_x_current = create_vector(Ny);
    // double* local_x_next = create_vector(Ny);
    // double* local_r_current = create_vector(Ny);
    // double* local_r_next = create_vector(Ny);
    // double* local_z_current = create_vector(Ny);
    // double* local_z_next = create_vector(Ny);
    // double* local_A_x0 = create_vector(Ny);
    // double* local_A_z_n = create_vector(Ny);
    // double* local_alpha_z_n = create_vector(Ny);
    // double* local_alpha_A_z_n = create_vector(Ny);
    // double* local_beta_z_n = create_vector(Ny);

    // double numerator = 0;
    // double denominator = 0;

    // double local_numerator;
    // double local_denominator;

    // copy_vector(local_x_current, local_x0, Ny);
    // multiply_vector_by_matrix(local_A, x0, Nx, Ny, local_A_x0);

    // subtract_vectors(local_b, local_A_x0, Ny, local_r_current);
    // copy_vector(local_z_current, local_r_current, Ny);

    // MPI_Gatherv(local_z_current, 
    //             Ny,
    //             MPI_DOUBLE,
    //             z_current,
    //             vector_sends_count,
    //             vector_displs,
    //             MPI_DOUBLE,
    //             ROOT_RANK,
    //             MPI_COMM_WORLD);

    // MPI_Bcast(z_current,
    //           Nx, 
    //           MPI_DOUBLE, 
    //           ROOT_RANK, 
    //           MPI_COMM_WORLD);

    // while (f) {
    //     multiply_vector_by_matrix(local_A, z_current, Nx, Ny, local_A_z_n);
    //     local_numerator = ddot(local_r_current, local_r_current, Ny);
    //     local_denominator = ddot(local_A_z_n, local_z_current, Ny);

    //     MPI_Allreduce(&local_numerator,
    //                   &numerator,
    //                   1,
    //                   MPI_DOUBLE,
    //                   MPI_SUM,
    //                   MPI_COMM_WORLD);

    //     MPI_Allreduce(&local_denominator,
    //                   &denominator,
    //                   1,
    //                   MPI_DOUBLE,
    //                   MPI_SUM,
    //                   MPI_COMM_WORLD);

    //     double alpha = numerator / denominator;




    //     multyply_vector_by_scalar(local_z_current, alpha, Ny, local_alpha_z_n);
    //     add_vectors(local_x_current, local_alpha_z_n, Ny, local_x_next);



    //     multyply_vector_by_scalar(local_A_z_n, alpha, Ny, local_alpha_A_z_n);
    //     subtract_vectors(local_r_current, local_alpha_A_z_n, Ny, local_r_next);






    //     denominator = numerator;

    //     local_numerator = ddot(local_r_next, local_r_next, Ny);

    //     MPI_Allreduce(&local_numerator,
    //                   &numerator,
    //                   1,
    //                   MPI_DOUBLE,
    //                   MPI_SUM,
    //                   MPI_COMM_WORLD);

    //     double beta = numerator / denominator;




    //     multyply_vector_by_scalar(local_z_current, beta, Ny, local_beta_z_n);
    //     add_vectors(local_r_next, local_beta_z_n, Ny, local_z_next);
    
    //     MPI_Gatherv(local_z_next, 
    //                 Ny,
    //                 MPI_DOUBLE,
    //                 z_next,
    //                 vector_sends_count,
    //                 vector_displs,
    //                 MPI_DOUBLE,
    //                 ROOT_RANK,
    //                 MPI_COMM_WORLD);

    //     copy_vector(local_x_current, local_x_next, Ny);
    //     copy_vector(local_r_current, local_r_next, Ny);
    
    //     if (root == ROOT_RANK) {
    //         copy_vector(z_current, z_next, Nx);
    //     }
    // }



    






    

    free(vector_sends_count);
    free(matrix_sends_count);
    free(vector_displs);
    free(matrix_displs);
    free(A);
    free(b);
    // free(x_current);
    // free(x_next);
    // free(r_current);
    // free(r_next);
    // free(z_current);
    // free(z_next);
    // // free(local_A);
    // free(local_x0);
    // free(local_x_current);
    // free(local_x_next);
    // free(local_r_current);
    // free(local_r_next);
    // free(local_z_current);
    // free(local_z_next);
    // free(local_A_x0);
    // free(local_A_z_n);
    // free(local_alpha_z_n);
    // free(local_alpha_A_z_n);
    // free(local_beta_z_n);

    MPI_Finalize();

    return 0;
}
