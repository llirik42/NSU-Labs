#include <malloc.h>
#include "matrix.h"

double* allocate_matrix(unsigned int rows, unsigned int columns) {
    return calloc(rows * columns, sizeof(double));
}

void set(double* matrix, unsigned int columns, unsigned int i, unsigned int j, double value) {
    matrix[i * columns + j] = value;
}

void destroy(void* ptr) {
    free(ptr);
}
