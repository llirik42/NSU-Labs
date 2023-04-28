#include <malloc.h>
#include <time.h>
#include "utils.h"
#include "config.h"

unsigned int iterate(const struct InputData* input_data, double* result) {
    const double* A = input_data->A;
    const double* b = input_data->b;
    const double* x0 = input_data->x0;
    const double epsilon = input_data->epsilon;
    const unsigned int n = input_data->n;
    const unsigned int max_iterations_count = input_data->max_iterations_count;

    unsigned int iterations_count = 0;
    double* z = allocate_vector(n);
    double* r = allocate_vector(n);
    double* A_z = allocate_vector(n);

    double alpha_numerator;
    double alpha;
    double beta;
    double Az_z_ddot;
    double b_nrm_epsilon_squared = 0;
    double r_nrm_squared = 0;

    unsigned int checks_count = 0;

    // Calculating |b|^2 * epsilon^2
    for (unsigned int i = 0; i < n; i++) {
        b_nrm_epsilon_squared += b[i] * b[i];
    }
    b_nrm_epsilon_squared *= epsilon * epsilon;

    // Calculating r_0, z_0 and (r_0, r_0)
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            r[i] += A[i * n + j] * x0[j];
        }

        r[i] = b[i] - r[i];
        z[i] = r[i];
        r_nrm_squared += r[i] * r[i];
    }

    while (1) {    
        if (r_nrm_squared < b_nrm_epsilon_squared) {
            checks_count++;
        } 
        else {
            checks_count = 0;
        }
        if (iterations_count > max_iterations_count || checks_count > CHECKS_COUNT) {
            break;
        }

        // Calculating A x z_n
        for (unsigned int i = 0; i < n; i++) {
            A_z[i] = 0;

            for (unsigned int j = 0; j < n; j++) {
                A_z[i] += A[i * n + j] * z[j];
            }
        }

        // Calculating alpha
        alpha_numerator = r_nrm_squared;
        Az_z_ddot = 0;
        for (unsigned int i = 0; i < n; i++) {
            Az_z_ddot += A_z[i] * z[i];
        }
        alpha = alpha_numerator / Az_z_ddot;
      
        // Calculating x_{n+1} and r_{n+1}
        for (unsigned int i = 0; i < n; i++) {
            result[i] += z[i] * alpha;
            r[i] = r[i] -  A_z[i] * alpha;
        }

        // Calculating beta
        r_nrm_squared = 0;
        for (unsigned int i = 0; i < n; i++) {
            r_nrm_squared += r[i] * r[i];
        }
        beta = r_nrm_squared / alpha_numerator;

        // Calculating z_{n+1}
        for (unsigned int i = 0; i < n; i++) {
            z[i] = z[i] * beta + r[i];
        }
            
        iterations_count++;
    }
    
    free(z);
    free(r);
    free(A_z);

    return iterations_count;
}

int main() {
    const double start_time = (double) clock();

    const struct InputData input_data = create_input_data();
    double* result = allocate_vector(input_data.n);
    unsigned int iterations_count = iterate(&input_data, result);

    const double end_time = (double) clock();

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
