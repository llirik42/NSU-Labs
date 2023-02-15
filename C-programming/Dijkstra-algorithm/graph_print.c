#include "graph_print.h"
#include <stdio.h>

#define TABLE_STYLE "align=\"center\"; border=\"2\"; width=\"800\"; height=\"800\";"
#define DEFAULT_CELL_STYLE "align=\"center\"; width=\"80\"; height=\"80\"; style=\"font-size: 200%\""
#define HELPER_CELL_STYLE "align=\"center\"; width=\"80\"; height=\"80\"; style=\"font-size: 120%\""

void makeIndent(FILE* file, const unsigned int indent){
    for (unsigned int i = 0; i < indent * 2; i++){
        fprintf(file, " ");
    }
}
void printString(FILE* file, const char* string, const unsigned int indent){
    makeIndent(file, indent);

    fprintf(file, "%s\n", string);
}
void printNumber(FILE* file, const unsigned int number, const unsigned int indent){
    makeIndent(file, indent);

    fprintf(file, "%u\n", number);
}
void openTag(FILE* file, const char tag[], const unsigned int indent, const char* params){
    makeIndent(file, indent);
    if (params){
        fprintf(file, "<%s %s>\n", tag, params);
    }
    else{
        fprintf(file, "<%s>\n", tag);
    }
}
void closeTag(FILE* file, const char tag[], const unsigned int indent){
    makeIndent(file, indent);
    fprintf(file, "</%s>\n", tag);
}
void printHTMLBeginning(FILE* file) {
    openTag(file, "head", 0, 0);
    openTag(file, "title", 1, 0);
    closeTag(file, "title", 1);
    closeTag(file, "head", 0);
    openTag(file, "body", 0, 0);
}
void printHTMLEnd(FILE* file){
    closeTag(file, "body", 0);
    closeTag(file, "html", 0);
}
void printRow(FILE * file, const struct AdjacencyMatrix* adjacencyMatrix, const unsigned int rowIndex){
    openTag(file, "tr", 3, 0);

    openTag(file, "td", 4, HELPER_CELL_STYLE);
    printNumber(file, rowIndex + 1, 5);
    closeTag(file, "td", 4);

    for (unsigned int i = 0; i < adjacencyMatrix->verticesCount; i++){
        const struct Distance curDistance = getEdge(adjacencyMatrix, i, rowIndex);

        openTag(file, "td", 4, DEFAULT_CELL_STYLE);
        makeIndent(file, 5);
        printDistance(curDistance, file);
        printString(file, "", 0);
        closeTag(file, "td", 4);
    }

    closeTag(file, "tr", 3);
}
void printTable(FILE* file, const struct AdjacencyMatrix* adjacencyMatrix){
    for (unsigned int i = 0; i < adjacencyMatrix->verticesCount; i++){
        printRow(file, adjacencyMatrix, i);
    }
}
void printHeaders(FILE* file, const unsigned int verticesCount){
    openTag(file, "tr", 3, 0);

    openTag(file, "td", 4, HELPER_CELL_STYLE);
    printString(file, "&nbsp", 5);
    closeTag(file, "td", 4);

    for (unsigned int i = 1; i <= verticesCount; i++){
        openTag(file, "td", 4, HELPER_CELL_STYLE);
        printNumber(file, i, 5);
        closeTag(file, "td", 4);
    }

    closeTag(file, "tr", 3);
}
void printAdjacencyMatrix(FILE* file, const struct AdjacencyMatrix* adjacencyMatrix){
    openTag(file, "table", 1, TABLE_STYLE);
    
    printHeaders(file, adjacencyMatrix->verticesCount);

    printTable(file, adjacencyMatrix);

    closeTag(file, "table", 2);
}
void PrintHTML(const struct AdjacencyMatrix* adjacencyMatrix){
    FILE* file = fopen("../graph.html", "w");

    printHTMLBeginning(file);

    printAdjacencyMatrix(file, adjacencyMatrix);

    printHTMLEnd(file);
}

void printVertices(FILE* file, const unsigned int verticesCount){
    for (unsigned int i = 0; i < verticesCount; i++){
        makeIndent(file, 2);
        fprintf(file, "<node id=\"%d\"/>\n", i + 1);
    }
}
void printEdges(FILE* file, const struct AdjacencyMatrix* adjacencyMatrix){
    unsigned int edgesCount = 0;
    for (unsigned int i = 0; i < adjacencyMatrix->verticesCount; i++){
        for (unsigned int j = 0; j < adjacencyMatrix->verticesCount; j++){
            const struct Distance curDistance = getEdge(adjacencyMatrix, i, j);

            if (isDistanceInfinity(curDistance)){
                continue;
            }

            makeIndent(file, 2);
            fprintf(file, "<edge id=\"e%d\" source=\"%d\" target=\"%d\"/>\n", edgesCount, i + 1, j + 1);

            edgesCount++;
        }
    }
}
void PrintGraphML(const struct AdjacencyMatrix* adjacencyMatrix){
    FILE* file = fopen("../graph.graphml", "w");

    openTag(file, "graphml", 0, 0);

    openTag(file, "graph", 1, "edgedefault=\"undirected\"");

    printVertices(file, adjacencyMatrix->verticesCount);

    printEdges(file, adjacencyMatrix);

    closeTag(file, "graph", 1);

    closeTag(file, "graphml", 0);
}
