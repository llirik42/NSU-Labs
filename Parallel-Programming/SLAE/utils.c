#include <malloc.h>
#include <xmmintrin.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "config.h"

#define DOUBLE_VALUES_IN_MM_128 2

double* create_vector(unsigned int size) {
    return malloc(sizeof(double) * size);
}

int* create_int_vector(unsigned int size) {
    return malloc(sizeof(int) * size);
}

double* create_matrix(unsigned int x_size, unsigned int y_size) {
    return malloc(sizeof(double) * x_size * y_size);
}

void copy_vector(const double* from, double* to, unsigned int size) {
    memcpy(to, from, sizeof(double) * size);
}

double ddot(const double* vector1, const double* vector2, unsigned int size) {
    const __m128d* x = (void*) vector1;
    const __m128d* y = (void*) vector2;
    __m128d result_vector = _mm_setzero_pd();

    const unsigned int part1 = size / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = size % DOUBLE_VALUES_IN_MM_128;

    for (unsigned int i = 0; i < part1; i++) {
        result_vector = _mm_add_pd(result_vector, _mm_mul_pd(*(x + i), *(y + i)));
    }

    double result = ((double*) &result_vector)[0] + ((double*) &result_vector)[1];

    for (unsigned int i = 0; i < part2; i++) {
        result += vector1[size - 1] * vector2[size - 1];
    }

    return result;
}

void multiply_vector_by_matrix(const double* matrix, const double* vector, unsigned int x_size, unsigned int y_size, double* result) {
    for (unsigned int i = 0; i < y_size; i++) {
	   result[i] = ddot(vector, matrix + i * x_size, x_size);
    }
}

void add_vectors(double* result, const double* delta, unsigned int size) {
    const __m128d* x = (void*) result;
    const __m128d* y = (void*) delta;

    const unsigned int part1 = size / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = size % DOUBLE_VALUES_IN_MM_128;

    for (unsigned int i = 0; i < part1; i++) {
        const __m128d tmp = _mm_add_pd(*(x + i), *(y + i));
        _mm_storeu_pd(result + i * DOUBLE_VALUES_IN_MM_128, tmp);
    }

    for (unsigned int i = 0; i < part2; i++) {
        result[size - 1] += delta[size - 1];
    }
}

void subtract_vectors(double* result, const double* subtrahend, unsigned int size) {
    const __m128d* x = (void*) result;
    const __m128d* y = (void*) subtrahend;

    const unsigned int part1 = size / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = size % DOUBLE_VALUES_IN_MM_128;

    for (unsigned int i = 0; i < part1; i++) {
        const __m128d tmp = _mm_sub_pd(*(x + i), *(y + i));
        _mm_storeu_pd(result + i * DOUBLE_VALUES_IN_MM_128, tmp);
    }

    for (unsigned int i = 0; i < part2; i++) {
        result[size - 1] -= subtrahend[size - 1];
    }
}
void inverse_subtract_vectors(double* result, const double* minuend, unsigned int size) {
    const __m128d* x = (void*) minuend;
    const __m128d* y = (void*) result;

    const unsigned int part1 = size / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = size % DOUBLE_VALUES_IN_MM_128;

    for (unsigned int i = 0; i < part1; i++) {
        const __m128d tmp = _mm_sub_pd(*(x + i), *(y + i));
        _mm_storeu_pd(result + i * DOUBLE_VALUES_IN_MM_128, tmp);
    }

    for (unsigned int i = 0; i < part2; i++) {
        result[size - 1] = minuend[size - 1] - result[size - 1];
    }
}

void multyply_vector_by_scalar(double* vector, double scalar, unsigned int size) {
    const __m128d* x = (void*) vector;
    const __m128d y = _mm_load_pd1(&scalar);

    const unsigned int part1 = size / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = size % DOUBLE_VALUES_IN_MM_128;

    for (unsigned int i = 0; i < part1; i++) {
        const __m128d tmp = _mm_mul_pd(*(x + i), y);
        _mm_storeu_pd(vector + i * DOUBLE_VALUES_IN_MM_128, tmp);
    }

    for (unsigned int i = 0; i < part2; i++) {
        vector[size - 1] *= scalar;
    }
}
void multyply_vector_by_scalar2(const double* vector, double scalar, unsigned int size, double* result) {
    const __m128d* x = (void*) vector;
    const __m128d y = _mm_load_pd1(&scalar);

    const unsigned int part1 = size / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = size % DOUBLE_VALUES_IN_MM_128;

    for (unsigned int i = 0; i < part1; i++) {
        const __m128d tmp = _mm_mul_pd(*(x + i), y);
        _mm_storeu_pd(result + i * DOUBLE_VALUES_IN_MM_128, tmp);
    }

    for (unsigned int i = 0; i < part2; i++) {
        result[size - 1] = vector[size - 1] * scalar;
    }
}

struct InputData create_input_data() {
    double* A = create_matrix(N, N);
    double* b = create_vector(N);
    double* x0 = create_vector(N);

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
    double* tmp = create_vector(N);
    multiply_vector_by_matrix(A, result, N, N, tmp);
    subtract_vectors(tmp, b, N);
    const double delta = sqrt(ddot(tmp, tmp, N));
    free(tmp);
    
    if (isnan(delta) || isinf(delta)) {
        printf("The proccess diverged\n");
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
