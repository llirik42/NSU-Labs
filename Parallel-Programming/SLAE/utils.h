#pragma once

#include <stddef.h>
#include <stdbool.h>

struct InputData {
    double* A;
    double* b;
    double* x0;
    double epsilon;
    size_t N;
    size_t max_iterations_count;
};

struct OutputData {
    double* result_vector;
    size_t iterations_count;
    bool converged;
};

double* create_vector(size_t N);
double* create_matrix(size_t Nx, size_t Ny);
void copy_vector(double* dest, const double* src, size_t N);
double ddot(const double* vector1, const double* vector2, size_t N);
void multiply_vector_by_matrix(const double* matrix, const double* vector, size_t Nx, size_t Ny, double* result);
void add_vectors(const double* vector1, const double* vector2, size_t N, double* result);
void subtract_vectors(const double* vector1, const double* vector2, size_t N, double* result);
void multyply_vector_by_scalar(const double* vector, double scalar, size_t N, double* result);
double calculate_nrm_squared(const double* vector, size_t N);

struct InputData create_input_data();
struct OutputData create_output_data(double* result_vector, size_t iterations_count, bool converged);
void destroy_input_data(struct InputData* input_data);
void destroy_output_data(struct OutputData* output_data);

void print_result_info(const struct InputData* input_data, const struct OutputData* output_data);

void print_time(double start_time, double end_time);
