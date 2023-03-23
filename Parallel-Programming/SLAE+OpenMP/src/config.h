#pragma once

#define N 500

#define EPSILON 0.00000001

#define MAX_ITERATIONS_COUNT 50000

double get_A_ij(unsigned int i, unsigned int j) {
	return i == j ? i + j : 1 + N;
}

double get_b_i(unsigned int i) {
	return (double) i;
}

double get_x0_i(unsigned int i) {
	return 0;
}
