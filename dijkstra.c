#include "Graph.h"

void DIJKSTRA(struct Graph *graph, char startID)
{
    bool visited[256] = {false};
    int distance[256];
    int allVertex = countAllVertexInGraph(graph);
    int allVisitedVertex = 0;

    for (int i = 0; i < 256; i++)
    {
        distance[i] = 32000;
    }

    distance[startID] = 0;

    while (allVisitedVertex != allVertex)
    {

        char currentID = -1;
        int minDist = 32000;

        struct VertexNode *currentNode = graph->allVertex;
        while (currentNode)
        {

            if (!visited[currentNode->vertex->ID] && distance[currentNode->vertex->ID] < minDist)
            {
                minDist = distance[currentNode->vertex->ID];
                currentID = currentNode->vertex->ID;
            }
            currentNode = currentNode->next;
        }

        if (currentID == -1)
        {
            break;
        }

        visited[currentID] = true;
        allVisitedVertex++;
        Vertex *currentVertex = NULL;
        currentNode = graph->allVertex;
        while (currentNode)
        {
            if (currentNode->vertex->ID == currentID)
            {
                currentVertex = currentNode->vertex;
                break;
            }
            currentNode = currentNode->next;
        }

        if (!currentVertex)
        {
            break;
        }

        struct EdgeNode *edgeNode = currentVertex->outEdge;
        while (edgeNode)
        {
            char toID = edgeNode->edge->to->ID;
            int newDist = distance[currentID] + edgeNode->edge->weight;
            if (newDist < distance[toID])
            {
                distance[toID] = newDist;
            }
            edgeNode = edgeNode->next;
        }
    }

    struct VertexNode *node = graph->allVertex;
    while (node)
    {
        char vertexID = node->vertex->ID;
        if (vertexID != startID)
        {
            printf("%c %d\n", vertexID, distance[vertexID]);
        }
        node = node->next;
    }
}