#include <malloc.h>
#include <assert.h>
#include <math.h>
#include "matrix.h"

#define EPSILON 10e-9

struct Matrix* allocate_matrix(size_t rows, size_t columns) {
    struct Matrix* matrix = malloc(sizeof(struct Matrix*));

    if (matrix == NULL) {
        return NULL;
    }

    matrix->rows = rows;
    matrix->columns = columns;
    matrix->elements = calloc(rows * columns, sizeof(double));

    if (matrix->elements == NULL) {
        destroy_matrix(matrix);
        return NULL;
    }

    return matrix;
}

struct Matrix* read_matrix(const char* path) {
    assert(path != NULL);

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Incorrect file for matrix!\n");
        return NULL;
    }

    size_t local_rows, local_columns;
    fscanf(file, "%zu %zu", &local_rows, &local_columns);

    struct Matrix* matrix = allocate_matrix(local_rows, local_columns);
    if (matrix == NULL) {
        fclose(file);
        return NULL;
    }

    for (size_t i = 0; i < local_rows; i++) {
        for (size_t j = 0; j < local_columns; j++) {
            fscanf(file, "%lf", &matrix->elements[j + i * local_columns]);
        }
    }

    matrix->rows = local_rows;
    matrix->columns = local_columns;

    fclose(file);

    return matrix;
}

void dump_maitrix(const struct Matrix* matrix, FILE* file) {
    assert(matrix != NULL);
    assert(file != NULL);

    fprintf(file, "%zu %zu\n", matrix->rows, matrix->columns);
    show_matrix(file, matrix);
}

void show_matrix(FILE* file, const struct Matrix* matrix) {
    assert(file != NULL);
    assert(matrix != NULL);

    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->columns; j++) {
            printf("%-8.1f ", get(matrix, i, j));
        }
        printf("\n");
    }
    printf("\n\n");
}

double get(const struct Matrix* matrix, size_t i, size_t j) {
    assert(matrix != NULL);
    assert(i < matrix->rows);
    assert(j < matrix->columns);

    return matrix->elements[i * matrix->columns + j];
}

void set(struct Matrix* matrix, size_t i, size_t j, double value) {
    assert(matrix != NULL);
    assert(i < matrix->rows);
    assert(j < matrix->columns);

    matrix->elements[i * matrix->columns + j] = value;
}

bool equal(const struct Matrix* matrix1, const struct Matrix* matrix2) {
    assert(matrix1 != NULL);
    assert(matrix2 != NULL);

    if (matrix1->columns != matrix2->columns || matrix1->rows != matrix2->rows) {
        return false;
    }

    for (size_t i = 0; i < matrix1->rows; i++) {
        for (size_t j = 0; j < matrix1->columns; j++) {
            const double a = get(matrix1, i, j);
            const double b = get(matrix2, i, j);

            if (fabs(a - b) >= EPSILON) {
                return false;
            }
        }
    }

    return true;
}

void destroy_matrix(struct Matrix* matrix) {
    if (matrix != NULL) {
        free(matrix->elements);
    }

    free(matrix);
}
