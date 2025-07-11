#include "Edge.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Graph
{
    struct VertexNode *allVertex;
    struct EdgeNode *allEdge;
};
struct Graph *addNODE(char *data, struct Graph *graph);
struct Graph *addEdge(char **data, struct Graph *graph);
Vertex *makeVertex(char id);
struct Graph *removeEdge(char ID1, char ID2, struct Graph *graph);

struct Graph *removeNode(char id, struct Graph *graph);
void RpoNumbering(struct Graph *graph, char startID);
void printGraph(struct Graph *graph);
int countAllVertexInGraph(struct Graph *graph);
void DIJKSTRA(struct Graph *graph, char startID);
void maxFlow(struct Graph *graph, char startID, char endID);
