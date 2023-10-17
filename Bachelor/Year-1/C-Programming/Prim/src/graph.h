#pragma once

#include "utils.h"

struct Graph;

void DestroyGraph(struct Graph* graph);
unsigned int GetEdgeLength(const struct Graph* graph, Vertex v1, Vertex v2);
unsigned int GetGraphVerticesCount(const struct Graph* graph);
Status InputGraph(struct Graph** pGraph);
