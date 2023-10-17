#pragma once

#include "utils.h"
#include "edge.h"
#include <stdbool.h>

struct SpanningTree;

struct SpanningTree* CreateSpanningTree(unsigned int verticesCount);
void DestroySpanningTree(struct SpanningTree* spanningTree);
void AddVertexToSpanningTree(struct SpanningTree* spanningTree, Vertex vertex);
bool InSpanningTree(const struct SpanningTree* spanningTree, Vertex vertex);
void AddEdgeToSpanningTree(struct SpanningTree* spanningTree, struct Edge edge);
void PrintSpanningTree(const struct SpanningTree* spanningTree);
