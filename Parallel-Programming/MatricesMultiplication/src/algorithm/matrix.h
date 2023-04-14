#pragma once

#include <stdbool.h>

struct Matrix {
    double* elements;
    size_t rows;
    size_t columns;
};

double* allocate_vector(size_t length);

struct Matrix* allocate_matrix(size_t rows, size_t columns);

struct Matrix* read_matrix(const char* path);

void dump_maitrix(const struct Matrix* matrix, FILE* file);

void show_matrix(FILE* file, const struct Matrix* matrix);

double get(const struct Matrix* matrix, size_t i, size_t j);

void set(struct Matrix* matrix, size_t i, size_t j, double value);

bool equal(const struct Matrix* matrix1, const struct Matrix* matrix2);

void destroy_matrix(struct Matrix* matrix);
