#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <time.h>

#include "utils.h"

struct OutputData calculate(const struct InputData* input_data) {
    const double* A = input_data->A;
    const double* b = input_data->b;
    const double* x0 = input_data->x0;
    const double epsilon = input_data->epsilon;
    const size_t N = input_data->N;
    const size_t max_iterations_count = input_data->max_iterations_count;

    const double epsilon_squared = epsilon * epsilon;

    double* x_current = create_vector(N);
    double* x_next = create_vector(N);
    copy_vector(x_current, x0, N);

    double* r_current = create_vector(N);
    double* r_next = create_vector(N);

    double* z_current = create_vector(N);
    double* z_next = create_vector(N);

    double* A_x0 = create_vector(N);
    multiply_vector_by_matrix(A, x_current, N, N, A_x0); // A * x_0

    subtract_vectors(b, A_x0, N, r_current); // r_0 = b - A * x_0

    copy_vector(z_current, r_current, N); // z_0 = r_0

    double* A_z_n = create_vector(N);
    double* alpha_z_n = create_vector(N);
    double* alpha_A_z_n = create_vector(N);
    double* beta_z_n = create_vector(N);

    const double b_nrm_squared = calculate_nrm_squared(b, N); // |b|^2

    const double epsilon_time_b_nrm_squared = epsilon_squared * b_nrm_squared; // epsilon^2 * |b|^2

    size_t current_iteration = 1;
    bool reached_precision = false;
    bool converged = true;

    while(!reached_precision) {
        multiply_vector_by_matrix(A, z_current, N, N, A_z_n); // A * z_n
        const double r_current_nrm_squared = ddot(r_current, r_current, N); // (r_n, r_n)
        const double alpha = r_current_nrm_squared / ddot(A_z_n, z_current, N); // alpha = (r_n, r_n) / (Az_n, z_n)

        multyply_vector_by_scalar(z_current, alpha, N, alpha_z_n); // alpha * z_n
        add_vectors(x_current, alpha_z_n, N, x_next); // x_{n+1} = x_n + alpha * z_n

        multyply_vector_by_scalar(A_z_n, alpha, N, alpha_A_z_n); // alpha * (A x z_n)
        subtract_vectors(r_current, alpha_A_z_n, N, r_next); // r_{n+1} = r_n - alpha * (A * z_n)

        const double beta = ddot(r_next, r_next, N) / r_current_nrm_squared; // beta = (r_{n+1}, r_{n+1}) / (r_n, r_n)

        multyply_vector_by_scalar(z_current, beta, N, beta_z_n); // beta * z_n
        add_vectors(r_next, beta_z_n, N, z_next); // z_{n+1} = r_{n+1} + beta * z_n

        reached_precision = calculate_nrm_squared(r_next, N) < epsilon_time_b_nrm_squared;

        copy_vector(x_current, x_next, N);
        copy_vector(r_current, r_next, N);
        copy_vector(z_current, z_next, N);
        
        current_iteration++;
    
        if (current_iteration > max_iterations_count) {
            converged = false;
            break;
        }
    }

    struct OutputData ret;

    if (!converged) {
        ret = create_output_data(NULL, 0, false);
    }
    else{
        double* result_vector = create_vector(N);
        copy_vector(result_vector, x_current, N);

        ret = create_output_data(result_vector, current_iteration, true);
    }

    free(x_current);
    free(x_next);
    free(r_current);
    free(r_next);
    free(z_current);
    free(z_next);
    free(A_x0);
    free(A_z_n);
    free(alpha_z_n);
    free(alpha_A_z_n);
    free(beta_z_n);

    return ret;
}

int main() {
	const time_t start_time = clock();

    struct InputData input_data = create_input_data();

    struct OutputData output_data = calculate(&input_data);

    const time_t end_time = clock();

    print_time(start_time / 1000, end_time / 1000);

    print_result_info(&input_data, &output_data);

    destroy_input_data(&input_data);
    destroy_output_data(&output_data);

    return 0;
}
