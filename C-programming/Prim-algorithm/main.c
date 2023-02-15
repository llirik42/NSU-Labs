#include "utils.h"
#include "graph.h"
#include "spanning_tree.h"
#include "edges_finder.h"

Status PrimAlgorithm(struct Graph* graph){
    struct SpanningTree* spanningTree;
    struct EdgesFinder* edgesFinder;

    const unsigned int verticesCount = GetGraphVerticesCount(graph);
    if (!verticesCount){
        return NO_SPANNING_TREE;
    }

    if (verticesCount == 1){
        return SUCCESS;
    }

    spanningTree = CreateSpanningTree(verticesCount);
    if (!spanningTree){
        return NO_MEMORY;
    }

    edgesFinder = CreateEdgesFinder(graph, spanningTree);
    if (!edgesFinder){
        DestroySpanningTree(spanningTree);
        return NO_MEMORY;
    }

    AddVertexToSpanningTree(spanningTree, 0);
    AdjustEdges(edgesFinder, 0);

    for (unsigned int i = 0; i < verticesCount - 1; i++){
        struct Edge shortestEdge;
        Vertex vertexWithShortestEdge;

        const bool foundShortestEdge = FindShortestEdge(edgesFinder, &shortestEdge, &vertexWithShortestEdge);

        if (!foundShortestEdge){
            break;
        }

        AddEdgeToSpanningTree(spanningTree, shortestEdge);

        AdjustEdges(edgesFinder, vertexWithShortestEdge);
    }

    PrintSpanningTree(spanningTree);

    DestroySpanningTree(spanningTree);
    DestroyEdgesFinder(edgesFinder);

    return SUCCESS;
}

int main() {
    struct Graph* graph;

    const Status statusOfInput = InputGraph(&graph);
    if (statusOfInput != SUCCESS){
        PrintStatus(statusOfInput);
        return 0;
    }

    const Status algorithmStatus = PrimAlgorithm(graph);
    if (algorithmStatus != SUCCESS){
        PrintStatus(algorithmStatus);
    }

    DestroyGraph(graph);

    return 0;
}
