#pragma once

struct InputData {
    double* A;
    double* b;
    double* x0;
    double epsilon;
    unsigned int n;
    unsigned int max_iterations_count;
};

double* allocate_vector(unsigned int size);
double* allocate_matrix(unsigned int x_size, unsigned int y_size);

struct InputData create_input_data();

void print_result_info(double start_time, double end_time, unsigned int iterations_count, const double* A, const double* b, const double* result);
