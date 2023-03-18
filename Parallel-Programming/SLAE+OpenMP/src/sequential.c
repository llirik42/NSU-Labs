#include <malloc.h>
#include <time.h>
#include "utils.h"

unsigned int iterate(const struct InputData* input_data, double* result) {
    const double* A = input_data->A;
    const double* b = input_data->b;
    const double* x0 = input_data->x0;
    const double epsilon = input_data->epsilon;
    const unsigned int N = input_data->n;
    const unsigned int max_iterations_count = input_data->max_iterations_count;

    unsigned int iterations_count = 0;
    double* z = create_vector(N);
    double* r = create_vector(N);
    double* A_z = create_vector(N);
    double* buffer = create_vector(N);

    const double b_nrm_epsilon_squared = ddot(b, b, N) * epsilon * epsilon;

    // Calculating r_0 and z_0
    multiply_vector_by_matrix(A, x0, N, N, r);
    inverse_subtract_vectors(r, b, N);
    copy_vector(r, z, N);

    // Calculating (r_0, r_0)
    double r_nrm_squared = ddot(r, r, N);

    while (1) {    
        if (r_nrm_squared < b_nrm_epsilon_squared || iterations_count > max_iterations_count) {
            break;
        }

        // Calculating A x z_n
        multiply_vector_by_matrix(A, z, N, N, A_z);

        // Calculating alpha
        const double alpha_numerator = r_nrm_squared;
        const double alpha = alpha_numerator / ddot(A_z, z, N);

        // Calculating x_{n+1}
        multiply_vector_by_scalar(z, alpha, N, buffer);
        add_vectors(result, buffer, N);

        // Calculating r_{n+1}
        multiply_vector_by_scalar(A_z, alpha, N, buffer);
        subtract_vectors(r, buffer, N);

        // Calculating beta
        r_nrm_squared = ddot(r, r, N);
        const double beta = r_nrm_squared / alpha_numerator;

        // Calculating z_{n+1}
        multiply_vector_by_scalar(z, beta, N, z);
        add_vectors(z, r, N);

        iterations_count++;
    }

    free(z);
    free(r);
    free(A_z);
    free(buffer);

    return iterations_count;
}

int main() {
    const time_t start_time = clock();

    const struct InputData input_data = create_input_data();
    double* result = create_vector(input_data.n);
    unsigned int iterations_count = iterate(&input_data, result);

    const time_t end_time = clock();

    print_result_info(start_time / 1000, 
                      end_time / 1000, 
                      iterations_count, 
                      input_data.A, 
                      input_data.b, 
                      result);

    free(input_data.A);
    free(input_data.b);
    free(input_data.x0);
    free(result);

    return 0;
}
