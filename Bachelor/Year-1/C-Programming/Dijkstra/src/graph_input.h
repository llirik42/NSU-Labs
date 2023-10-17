#pragma once

#include <stdbool.h>
#include "adjacency_matrix.h"

bool inputGraphData(struct AdjacencyMatrix** adjacencyMatrix, Number* verticesCount, Number* sourceVertex,
                    Number* destinationVertex);
