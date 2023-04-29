#pragma once

#include <mpi/mpi.h>
#include "matrix.h"

struct AlgorithmData {
    int p1;
    int p2;
    int n1;
    int n2;
    int n3;
    double* a;
    double* b;
};

struct AlgorithmData get_algorithm_data();

void destroy_algorithm_data(struct AlgorithmData* algorithm_data);

MPI_Datatype get_algorithm_data_mpi_type();
