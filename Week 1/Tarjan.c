#include "Graph.h"

struct ComponentNode
{
    char *vertexs;
    struct ComponentNode *next;
};

void DFSTarjan(Vertex *current, int *index, struct VertexNode **stack, int *lowIndex, bool *inStack, int *globalIndex, struct ComponentNode **components)
{
    index[current->ID] = *globalIndex;
    lowIndex[current->ID] = *globalIndex;
    (*globalIndex)++;
    struct VertexNode *newNode = malloc(sizeof(struct VertexNode));
    newNode->vertex = current;
    newNode->next = *stack;
    *stack = newNode;
    inStack[current->ID] = true;
    struct EdgeNode *currentEdgeNode = current->outEdge;
    while (currentEdgeNode != NULL)
    {
        Vertex *soon = currentEdgeNode->edge->to;
        if (index[soon->ID] == -1)
        {
            DFSTarjan(soon, index, stack, lowIndex, inStack, globalIndex, components);
            if (lowIndex[current->ID] > lowIndex[soon->ID])
                lowIndex[current->ID] = lowIndex[soon->ID];
        }
        else
        {
            if (lowIndex[current->ID] > lowIndex[soon->ID])
                lowIndex[current->ID] = lowIndex[soon->ID];
        }
        currentEdgeNode = currentEdgeNode->next;
    }
    if (lowIndex[current->ID] == index[current->ID])
    {
        char componentVertex[256] = {0};
        int componentCount = 0;
        Vertex *w;
        struct VertexNode *node;
        while (w != current)
        {
            node = *stack;
            *stack = node->next;
            w = node->vertex;
            inStack[w->ID] = false;
            componentVertex[componentCount++] = w->ID;
            free(node);
        }
        if (componentCount > 1)
        {
            struct ComponentNode *componentNode = malloc(sizeof(struct ComponentNode));
            componentNode->vertexs = malloc(componentCount + 1);
            strncpy(componentNode->vertexs, componentVertex, componentCount);
            componentNode->vertexs[componentCount] = '\0';
            componentNode->next = *components;
            *components = componentNode;
        }
    }
}

void Tarjan(struct Graph *graph, char ID)
{
    struct VertexNode *currentNode = graph->allVertex;
    Vertex *startVertex = NULL;
    while (currentNode)
    {
        if (currentNode->vertex->ID == ID)
        {
            startVertex = currentNode->vertex;
        }
        currentNode = currentNode->next;
    }
    int index[256];
    struct VertexNode *stack = NULL;
    int lowIndex[256];
    bool inStack[256];
    int globalIndex = 0;
    struct ComponentNode *components = NULL;
    for (int i = 0; i < 256; i++)
    {
        index[i] = -1;
        lowIndex[i] = -1;
        inStack[i] = false;
    }
    currentNode = graph->allVertex;
    DFSTarjan(startVertex, index, &stack, lowIndex, inStack, &globalIndex, &components);
    struct ComponentNode *currentComp = components;
    while (currentComp != NULL)
    {
        printf("%s\n", currentComp->vertexs);
        currentComp = currentComp->next;
    }
}