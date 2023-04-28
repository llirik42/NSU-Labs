#include "adjacency_matrix.h"
#include "graph_input.h"
#include "dijkstra_algorithm.h"
#include "args.h"
#include "graph_print.h"

int main(int argc, char* argv[]) {
    struct AdjacencyMatrix* adjacencyMatrix = 0;
    Number verticesCount = 0;
    Number sourceVertex = 0;
    Number destinationVertex = 0;

    bool inputResponse = inputGraphData(&adjacencyMatrix, &verticesCount, &sourceVertex, &destinationVertex);
    if (!inputResponse){
        return 0;
    }

    if (checkHTML(argc, argv)){
        PrintHTML(adjacencyMatrix);
        destroyAdjacencyMatrix(adjacencyMatrix);
        return 0;
    }
    if (checkGraphML(argc, argv)){
        PrintGraphML(adjacencyMatrix);
        destroyAdjacencyMatrix(adjacencyMatrix);
        return 0;
    }

    struct DijkstraData* dijkstraData = createDijkstraData(adjacencyMatrix, verticesCount, sourceVertex,
                                                           destinationVertex);

    dijkstraAlgorithm(dijkstraData);

    printResult(dijkstraData);

    destroyDijkstraData(dijkstraData);

    return 0;
}
