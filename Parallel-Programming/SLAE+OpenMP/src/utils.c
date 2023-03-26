#include <malloc.h>
#include <math.h>
#include "utils.h"
#include "config.h"

double ddot_utils(const double* vector1, const double* vector2, unsigned int size) {
    double result = 0;    

    for (unsigned int i = 0; i < size; i++) {
        result += vector1[i] * vector2[i];
    }

    return result;
}

void multiply_vector_by_matrix_utils(const double* matrix, const double* vector, unsigned int x_size, unsigned int y_size, double* result) {
    for (unsigned int i = 0; i < y_size; i++) {
       result[i] = ddot_utils(vector, matrix + i * x_size, x_size);
    }
}

void subtract_vectors_utils(double* result, const double* subtrahend, unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        result[i] -= subtrahend[i];
    }
}

double* allocate_vector(unsigned int size) {
    return calloc(size, sizeof(double));
}

double* allocate_matrix(unsigned int x_size, unsigned int y_size) {
    return calloc(x_size * y_size, sizeof(double));
}

struct InputData create_input_data() {
    double* A = allocate_matrix(N, N);
    double* b = allocate_vector(N);
    double* x0 = allocate_vector(N);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[N * i + j] = GET_A_IJ(i, j);
        }

        b[i] = GET_B_I(i);
        x0[i] = GET_X0_I(i);
    }

    struct InputData ret = {
        .A = A,
        .b = b,
        .x0 = x0, 
        .epsilon = EPSILON, 
        .n = N, 
        .max_iterations_count = MAX_ITERATIONS_COUNT
    };

    return ret;
}

double calculate_delta(const double* A, const double* b, const double* result) {
    double* buffer = allocate_vector(N);
    multiply_vector_by_matrix_utils(A, result, N, N, buffer);
    subtract_vectors_utils(buffer, b, N);
    const double delta = sqrt(ddot_utils(buffer, buffer, N));
    free(buffer);
    return delta;
}

void print_result_info(double start_time, double end_time, unsigned int iterations_count, const double* A, const double* b, const double* result) {
    const double delta = calculate_delta(A, b, result);
    const double total_time_ms = end_time - start_time;

    printf("Converged                            %d\n", iterations_count < MAX_ITERATIONS_COUNT && !isnan(delta) && !isinf(delta));
    printf("Delta                                %f\n", delta);
    printf("Time (ms)                            %.0f\n", total_time_ms);
    printf("Iterations                           %u\n", iterations_count);
    printf("Average iteration time (ms)          %f\n", total_time_ms / (double) iterations_count);
}
