#pragma once

#define N 512

#define EPSILON 0.00000001

#define MAX_ITERATIONS_COUNT 50000

#define CHECKS_COUNT 100

#define GET_A_IJ(I, J) ((i) == (j) ? (i) + (j) : 1 + (N))

#define GET_B_I(I) ((double) i)

#define GET_X0_I(I) (0)
