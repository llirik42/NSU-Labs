#include <stddef.h>

size_t input_vectors_size();

void input_vectors(double* vector1, double* vector2);

double* create_vector(size_t size);

void destroy_vector(double* vector);

double operation(const double* vector1, const double* vector2, size_t size1, size_t size2);

void check_result(double result);

void print_time(double start_time, double end_time);
