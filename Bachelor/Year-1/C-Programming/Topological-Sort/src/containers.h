#pragma once

#include "utils.h"
#include <stdbool.h>

struct AdjacencyMatrix;
struct VerticesQueue;

struct AdjacencyMatrix* CreateAdjacencyMatrix(Number verticesCount);
void DestroyAdjacencyMatrix(struct AdjacencyMatrix* adjacencyMatrix);
Vertex GetVertexWithNoAncestors(const struct AdjacencyMatrix* adjacencyMatrix);
void AddEdge(struct AdjacencyMatrix* adjacencyMatrix, Vertex ancestor, Vertex descendant);
void DeleteEdgesFromVertex(struct AdjacencyMatrix* adjacencyMatrix, Vertex vertex);

struct VerticesQueue* CreateVerticesQueue(Number verticesCount);
void DestroyVerticesQueue(struct VerticesQueue* verticesQueue);
void Enqueue(struct VerticesQueue* verticesQueue, Vertex vertex);
Vertex Dequeue(struct VerticesQueue* verticesQueue);
