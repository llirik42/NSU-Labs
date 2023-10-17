#pragma once

#include <limits.h>

#define SUCCESS 0
#define BAD_NUMBER_OF_VERTICES 1
#define BAD_NUMBER_OF_EDGES 2
#define BAD_VERTEX 3
#define BAD_LENGTH 4
#define BAD_NUMBER_OF_LINES 5
#define NO_MEMORY 6
#define NO_SPANNING_TREE 7

#define INFINITE_LENGTH UINT_MAX

typedef unsigned int Vertex;
typedef unsigned char Status;

void PrintStatus(Status status);
