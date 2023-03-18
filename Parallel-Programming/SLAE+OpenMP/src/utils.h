#pragma once

struct InputData {
    double* A;
    double* b;
    double* x0;
    double epsilon;
    unsigned int n;
    unsigned int max_iterations_count;
};

double* create_vector(unsigned int size);
int* create_int_vector(unsigned int size);
double* create_matrix(unsigned int x_size, unsigned int y_size);
void copy_vector(const double* from, double* to, unsigned int size);
double ddot(const double* vector1, const double* vector2, unsigned int size);
void multiply_vector_by_matrix(const double* matrix, const double* vector, unsigned int x_size, unsigned int y_size, double* result);
void add_vectors(double* result, const double* delta, unsigned int size);
void subtract_vectors(double* result, const double* subtrahend, unsigned int size);
void inverse_subtract_vectors(double* result, const double* minuend, unsigned int size);
void multiply_vector_by_scalar(const double* vector, double scalar, unsigned int size, double* result);

struct InputData create_input_data();

void print_result_info(double start_time, double end_time, unsigned int iterations_count, const double* A, const double* b, const double* result);
