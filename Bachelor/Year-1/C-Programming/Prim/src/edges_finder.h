#pragma once

#include "graph.h"
#include "edge.h"
#include "spanning_tree.h"
#include <stdbool.h>

struct EdgesFinder;

struct EdgesFinder* CreateEdgesFinder(struct Graph* graph, struct SpanningTree* spanningTree);
void DestroyEdgesFinder(struct EdgesFinder* edgesFinder);
bool FindShortestEdge(const struct EdgesFinder* edgesFinder, struct Edge* shortestEdge, Vertex* vertex);
void AdjustEdges(struct EdgesFinder* edgesFinder, Vertex newVertex);
