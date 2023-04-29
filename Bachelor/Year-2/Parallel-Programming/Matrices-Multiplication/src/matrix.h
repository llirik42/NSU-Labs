#pragma once

double* allocate_matrix(unsigned int rows, unsigned int columns);

void set(double* matrix, unsigned int columns, unsigned int i, unsigned int j, double value);

void destroy(void* ptr);
