#include "graph_input.h"
#include "kruskal_algorithm.h"

int main(){
    unsigned int verticesCount = 0;
    unsigned int edgesCount = 0;
    struct EdgesArray* edgesArray = 0;

    if (!inputGraphData(&edgesArray, &verticesCount, &edgesCount)){
        return 0;
    }

    kruskalAlgorithm(edgesArray, verticesCount, edgesCount);

    destroyEdgesArray(edgesArray);

    return 0;
}
