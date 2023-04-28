#include <omp.h>
#include <string.h>
#include <xmmintrin.h>
#include "utils.h"
#include "config.h"

#define DOUBLE_VALUES_IN_MM_128 2

double default_ddot(const double* vector1, const double* vector2, unsigned int size, unsigned int offset) {
    double result = 0;

    for (unsigned int i = 0; i < size; i++) {
        result += vector1[i + offset] * vector2[i + offset];
    }

    return result;
}

void default_add_vectors(double* result, const double* delta, unsigned int size, unsigned int offset) {
    for (unsigned int i = 0; i < size; i++) {
        result[i + offset] += delta[i + offset];
    }
}

void default_subtract_vectors(double* result, const double* subtrahend, unsigned int size, unsigned int offset) {
    for (unsigned int i = 0; i < size; i++) {
        result[i + offset] -= subtrahend[i + offset];
    }
}

void default_inverse_subtract_vectors(double* result, const double* minuend, unsigned int size, unsigned int offset) {
    for (unsigned int i = 0; i < size; i++) {
        result[i + offset] = minuend[i + offset] - result[i + offset];
    }
}

void default_multiply_vector_by_scalar(const double* vector, double scalar, unsigned int size, unsigned int offset, double* result) {
    for (unsigned int i = 0; i < size; i++) {
        result[i + offset] = vector[i + offset] * scalar;
    }
}

double ddot(const double* vector1, const double* vector2, unsigned int size) {
    double result;

    __m128d x;
    __m128d y;
    __m128d tmp = _mm_setzero_pd();

    const unsigned int part1 = size / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = size % DOUBLE_VALUES_IN_MM_128;

    for (unsigned int i = 0; i < part1; i++) {
        x = _mm_loadu_pd(vector1 + i * 2);
        y = _mm_loadu_pd(vector2 + i * 2);
        tmp = _mm_add_pd(tmp, _mm_mul_pd(x, y));
    }

    result = ((double*) &tmp)[0] + ((double*) &tmp)[1];

    result += default_ddot(vector1, vector2, part2, size - 1);

    return result;
}

void multiply_vector_by_matrix(const double* matrix, const double* vector, unsigned int x_size, unsigned int y_size, double* result) {
    for (unsigned int i = 0; i < y_size; i++) {
        result[i] = ddot(vector, matrix + i * x_size, x_size);
    }
}

void copy_vector(const double* from, double* to, unsigned int size) {
    memcpy(to, from, sizeof(double) * size);
}

void add_vectors(double* result, const double* delta, unsigned int size) {
    __m128d x;
    __m128d y;
    __m128d tmp;

    const unsigned int part1 = size / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = size % DOUBLE_VALUES_IN_MM_128;

    for (unsigned int i = 0; i < part1; i++) {
        x = _mm_loadu_pd(result + i * 2);
        y = _mm_loadu_pd(delta + i * 2);
        tmp = _mm_add_pd(x, y);
        _mm_storeu_pd(result + i * DOUBLE_VALUES_IN_MM_128, tmp);
    }

    default_add_vectors(result, delta, part2, size - 1);
}

void subtract_vectors(double* result, const double* subtrahend, unsigned int size) {
    __m128d x;
    __m128d y;
    __m128d tmp;

    const unsigned int part1 = size / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = size % DOUBLE_VALUES_IN_MM_128;

    for (unsigned int i = 0; i < part1; i++) {
        x = _mm_loadu_pd(result + i * 2);
        y = _mm_loadu_pd(subtrahend + i * 2);
        tmp = _mm_sub_pd(x, y);
        _mm_storeu_pd(result + i * DOUBLE_VALUES_IN_MM_128, tmp);
    }

    default_subtract_vectors(result, subtrahend, part2, size - 1);
}

void inverse_subtract_vectors(double* result, const double* minuend, unsigned int size) {
    __m128d x;
    __m128d y;
    __m128d tmp;

    const unsigned int part1 = size / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = size % DOUBLE_VALUES_IN_MM_128;

    for (unsigned int i = 0; i < part1; i++) {
        x = _mm_loadu_pd(result + i * 2);
        y = _mm_loadu_pd(minuend + i * 2);
        tmp = _mm_sub_pd(y, x);
        _mm_storeu_pd(result + i * DOUBLE_VALUES_IN_MM_128, tmp);
    }

    default_inverse_subtract_vectors(result, minuend, part2, size - 1);
}

void multiply_vector_by_scalar(const double* vector, double scalar, unsigned int size, double* result) {
    __m128d x;
    __m128d tmp;
    const __m128d y = _mm_load_pd1(&scalar);

    const unsigned int part1 = size / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = size % DOUBLE_VALUES_IN_MM_128;

    for (unsigned int i = 0; i < part1; i++) {
        x = _mm_loadu_pd(vector + i * 2);
        tmp = _mm_mul_pd(x, y);
        _mm_storeu_pd(result + i * DOUBLE_VALUES_IN_MM_128, tmp);
    }

    default_multiply_vector_by_scalar(vector, scalar, part2, size - 1, result);
}

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
    double* buffer = allocate_vector(n);

    double alpha_numerator = 0;
    double alpha = 0;
    double beta = 0;
    double Az_z_ddot = 0;
    double b_nrm_epsilon_squared = 0;
    double r_nrm_squared = 0;

    unsigned int checks_count = 0;

    #pragma omp parallel default(none) shared(A, b, x0, epsilon, n, max_iterations_count, iterations_count, z, r, A_z, alpha_numerator, alpha, beta, Az_z_ddot, b_nrm_epsilon_squared, r_nrm_squared, checks_count, buffer, result)
    {
        const int num_threads = omp_get_num_threads();
        const int thread_num = omp_get_thread_num();

        const int k1 = thread_num * (int) n / num_threads;
        const int k2 = (thread_num + 1) * (int) n / num_threads;
        const int local_n = k2 - k1;

        // Calculating |b|^2 * epsilon^2
        double local_b_nrm_epsilon_squared = ddot(b + k1, b + k1, local_n);
        #pragma omp atomic
        b_nrm_epsilon_squared += local_b_nrm_epsilon_squared;
        #pragma omp barrier
        #pragma omp single
        {
            b_nrm_epsilon_squared *= epsilon * epsilon;
        }

        // Calculating r_0
        multiply_vector_by_matrix(A + k1 * n, x0, n, local_n, r + k1);
        inverse_subtract_vectors(r + k1, b + k1, local_n);

        // Calculating z_0 and
        copy_vector(r + k1, z + k1, local_n);

        // Calculating (r_0, r_0)
        double local_r_nrm_squared = ddot(r + k1, r + k1, local_n);
        #pragma omp atomic
        r_nrm_squared += local_r_nrm_squared;

        while (1) {
            #pragma omp single
            {
                if (r_nrm_squared < b_nrm_epsilon_squared) {
                    checks_count++;
                }
                else {
                    checks_count = 0;
                }
            }
            if (iterations_count > max_iterations_count || checks_count > CHECKS_COUNT) {
                break;
            }

            // Calculating A x z_n
            multiply_vector_by_matrix(A + k1 * n, z, n, local_n, A_z + k1);

            // Calculating alpha
            #pragma omp single
            {
                alpha_numerator = r_nrm_squared;
                Az_z_ddot = 0;
            }
            double local_Az_z_ddot = ddot(A_z + k1, z + k1, local_n);
            #pragma omp atomic
            Az_z_ddot += local_Az_z_ddot;
            #pragma omp barrier
            #pragma omp single
            {
                alpha = alpha_numerator / Az_z_ddot;
            }

            // Calculating x_{n+1} and r_{n+1}
            multiply_vector_by_scalar(z + k1, alpha, local_n, buffer + k1);
            add_vectors(result + k1, buffer + k1, local_n);
            multiply_vector_by_scalar(A_z + k1, alpha, local_n, buffer + k1);
            subtract_vectors(r + k1, buffer + k1, local_n);

            // Calculating beta
            #pragma omp single
            {
                r_nrm_squared = 0;
            }
            local_r_nrm_squared = ddot(r + k1, r + k1, local_n);
            #pragma omp atomic
            r_nrm_squared += local_r_nrm_squared;
            #pragma omp barrier
            #pragma omp single
            {
                beta = r_nrm_squared / alpha_numerator;
            }

            // Calculating z_{n+1}
            multiply_vector_by_scalar(z + k1, beta, local_n, z + k1);
            add_vectors(z + k1, r + k1, local_n);

            #pragma omp single
            {
                iterations_count++;
            }
        }
    }

    free(z);
    free(r);
    free(A_z);
    free(buffer);

    return iterations_count;
}

int main() {
    const double start_time = omp_get_wtime();

    const struct InputData input_data = create_input_data();
    double* result = allocate_vector(input_data.n);
    unsigned int iterations_count = iterate(&input_data, result);

    const double end_time = omp_get_wtime();

    print_result_info(start_time * 1000,
                      end_time * 1000,
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
