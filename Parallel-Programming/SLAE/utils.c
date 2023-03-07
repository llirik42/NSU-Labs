#include <malloc.h>
#include <xmmintrin.h>
#include <string.h>
#include <math.h>

#include "utils.h"

#define DOUBLE_VALUES_IN_MM_128 2

double* create_vector(size_t N) {
	return malloc(sizeof(double) * N);
}

double* create_matrix(size_t Nx, size_t Ny) {
	return malloc(sizeof(double) * Nx * Ny);
}

void copy_vector(double* dest, const double* src, size_t N) {
	memcpy(dest, src, sizeof(double) * N);
}

double ddot(const double* vector1, const double* vector2, size_t N) {
    const __m128d* x = (void*) vector1;
    const __m128d* y = (void*) vector2;
    __m128d result_vector = _mm_setzero_pd();

    const unsigned int part1 = N / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = N % DOUBLE_VALUES_IN_MM_128;

    for (size_t i = 0; i < part1; i++) {
        result_vector = _mm_add_pd(result_vector, _mm_mul_pd(*(x + i), *(y + i)));
    }

    double result = ((double*) &result_vector)[0] + ((double*) &result_vector)[1];

    for (size_t i = 0; i < part2; i++) {
        result += vector1[N - 1] * vector2[N - 1];
    }

    return result;
}

void multiply_vector_by_matrix(const double* matrix, const double* vector, size_t Nx, size_t Ny, double* result) {
	for (size_t i = 0; i < Ny; i++) {
		result[i] = ddot(vector, matrix + i * Nx, Nx);
	}
}

void add_vectors(const double* vector1, const double* vector2, size_t N, double* result) {
    const __m128d* x = (void*) vector1;
    const __m128d* y = (void*) vector2;

    const unsigned int part1 = N / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = N % DOUBLE_VALUES_IN_MM_128;

    for (size_t i = 0; i < part1; i++) {
        const __m128d tmp = _mm_add_pd(*(x + i), *(y + i));
        _mm_storeu_pd(result + i * DOUBLE_VALUES_IN_MM_128, tmp);
    }

    for (unsigned int i = 0; i < part2; i++) {
        result[N - 1] = vector1[N - 1] + vector2[N - 1];
    }
}

void subtract_vectors(const double* vector1, const double* vector2, size_t N, double* result) {
	const __m128d* x = (void*) vector1;
    const __m128d* y = (void*) vector2;

    const unsigned int part1 = N / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = N % DOUBLE_VALUES_IN_MM_128;

    for (size_t i = 0; i < part1; i++) {
        const __m128d tmp = _mm_sub_pd(*(x + i), *(y + i));
        _mm_storeu_pd(result + i * DOUBLE_VALUES_IN_MM_128, tmp);
    }

    for (unsigned int i = 0; i < part2; i++) {
        result[N - 1] = vector1[N - 1] - vector2[N - 1];
    }
}

void multyply_vector_by_scalar(const double* vector, double scalar, size_t N, double* result) {
    const __m128d* x = (void*) vector;
    const __m128d y = _mm_load_pd1(&scalar);

    const unsigned int part1 = N / DOUBLE_VALUES_IN_MM_128;
    const unsigned int part2 = N % DOUBLE_VALUES_IN_MM_128;

    for (unsigned int i = 0; i < part1; i++) {
        const __m128d tmp = _mm_mul_pd(*(x + i), y);
        _mm_storeu_pd(result + i * DOUBLE_VALUES_IN_MM_128, tmp);
    }

    for (unsigned int i = 0; i < part2; i++) {
        result[N - 1] = vector[N - 1] * scalar;
    }
}

double calculate_nrm_squared(const double* vector, size_t N) {
    return ddot(vector, vector, N);
}

struct InputData create_input_data() {
    const size_t N = 10;
    const double EPSILON = 0.000000001; // 10^(-9)
    const size_t MAX_ITERATIONS_COUNT = 50000;

    double* A = create_matrix(N, N);
    double* b = create_vector(N);
    double* x0 = create_vector(N);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[N * i + j] = i == j ? i + j : 1 + N;
        }

        x0[i] = 0;
        b[i] = i;
    }

    struct InputData ret = {
        .A = A,
        .b = b,
        .x0 = x0, 
        .epsilon = EPSILON, 
        .N = N, 
        .max_iterations_count = MAX_ITERATIONS_COUNT
    };
    return ret;
}

struct OutputData create_output_data(double* result_vector, size_t iterations_count, bool converged) {
    struct OutputData ret = {
        .result_vector = result_vector, 
        .iterations_count = iterations_count,
        .converged = converged
    };    
    return ret;
}

void destroy_input_data(struct InputData* input_data) {
    free(input_data->A);
    free(input_data->b);
    free(input_data->x0);
}

void destroy_output_data(struct OutputData* output_data) {
    free(output_data->result_vector);
}

void print_delta(const struct InputData* input_data, const struct OutputData* output_data) {
    const double* A = input_data->A;
    const double* b = input_data->b;
    const size_t N = input_data->N;
    const double* result_vector = output_data->result_vector;

    double* tmp = create_vector(N);
    multiply_vector_by_matrix(A, result_vector, N, N, tmp);

    subtract_vectors(tmp, b, N, tmp);

    const double delta = sqrt(calculate_nrm_squared(tmp, N));

    free(tmp);

    printf("|Ax - b| = %lf\n", delta);
}

void print_result_info(const struct InputData* input_data, const struct OutputData* output_data) {
	if (output_data->converged) {
        printf("Iterations: %lu\n", output_data->iterations_count);
        print_delta(input_data, output_data);
    }
    else {
        printf("The proccess didn't converge in %lu iterations\n", input_data->max_iterations_count);
    }
}

void print_time(double start_time, double end_time) {
    printf("Time: %.0f ms\n", end_time - start_time);
}
