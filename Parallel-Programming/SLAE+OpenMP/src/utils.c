#include <malloc.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "config.h"

double ddot(const double* vector1, const double* vector2, unsigned int size) {
    double result = 0;    

    for (unsigned int i = 0; i < size; i++) {
        result += vector1[i] * vector2[i];
    }

    return result;
}

void multiply_vector_by_matrix(const double* matrix, const double* vector, unsigned int x_size, unsigned int y_size, double* result) {
    for (unsigned int i = 0; i < y_size; i++) {
       result[i] = ddot(vector, matrix + i * x_size, x_size);
    }
}

void subtract_vectors(double* result, const double* subtrahend, unsigned int size) {
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
            A[N * i + j] = get_A_ij(i, j);
        }

        b[i] = get_b_i(i);
        x0[i] = get_x0_i(i);
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

void print_delta(const double* A, const double* b, const double* result) {
    double* buffer = allocate_vector(N);
    multiply_vector_by_matrix(A, result, N, N, buffer);
    subtract_vectors(buffer, b, N);
    const double delta = sqrt(ddot(buffer, buffer, N));
    free(buffer);
    
    if (isnan(delta) || isinf(delta)) {
        printf("The process diverged\n");
    } 
    else {
        printf("|Ax - b| = %lf\n", delta);
    }
}

void print_result_info(double start_time, double end_time, unsigned int iterations_count, const double* A, const double* b, const double* result) {
    printf("Time: %.0f ms\n", end_time - start_time);

    if (iterations_count >= MAX_ITERATIONS_COUNT) {
        printf("Didn't converge in %u iterations\n", MAX_ITERATIONS_COUNT);
    }
    else {
        printf("Iterations: %u\n", iterations_count);
        print_delta(A, b, result);
    }    
}
