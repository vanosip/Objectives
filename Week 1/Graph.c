#include "Graph.h"
/*Функция создаёт вершину по заданному ID */
Vertex *makeVertex(char id)
{
    Vertex *vertex = malloc(sizeof(Vertex));
    vertex->ID = id;
    vertex->inputEdge = NULL;
    vertex->outEdge = NULL;
    return vertex;
}
/*Функция создаёт новое ребро
vertexFirst - вершина от куда
vertexSecond - вершина приемник*/
Edge *makeEdge(Vertex *vertexFirst, Vertex *vertexSecond, int weight)
{
    Edge *edge = malloc(sizeof(Edge));
    edge->from = vertexFirst;
    edge->to = vertexSecond;
    edge->weight = weight;
    return edge;
}
/*Функция удаляет ребро из спика рёбер
node - список откуда следует удалить ребро
toRemove - ребро которое следует удалить*/
void removeEdgeFromEdgeNode(struct EdgeNode **node, Edge *toRemove)
{
    struct EdgeNode *current = *node;
    struct EdgeNode *prev = NULL;
    while (current != NULL)
    {
        if (current->edge == toRemove)
        {
            if (prev == NULL)
            {
                *node = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }
}
/*Функция добавляет ребро в граф
data - строка ID*/
struct Graph *addNODE(char *data, struct Graph *graph)
{
    if (graph == NULL)
    {
        graph = malloc(sizeof(struct Graph));
        graph->allVertex = NULL;
        graph->allEdge = NULL;
        Vertex *vertex = makeVertex(data[0]);
        struct VertexNode *vertexNode = malloc(sizeof(struct VertexNode));
        vertexNode->vertex = vertex;
        vertexNode->next = NULL;
        graph->allVertex = vertexNode;
    }
    else
    {
        struct VertexNode *lastVertex = NULL;
        for (lastVertex = graph->allVertex; lastVertex->next != NULL; lastVertex = lastVertex->next)
            ;
        Vertex *vertex = makeVertex(data[0]);
        struct VertexNode *currentNode = malloc(sizeof(struct VertexNode));
        currentNode->vertex = vertex;
        currentNode->next = NULL;
        lastVertex->next = currentNode;
    }
    return graph;
}
/*Функция добавляет ребро в граф, связывает вершины*/
struct Graph *addEdge(char **data, struct Graph *graph)
{
    if (!graph)
    {
        printf("Unknown nodes %c %c\n", data[1][0], data[2][0]);
        return NULL;
    }
    struct VertexNode *currentVertex = NULL;
    Vertex *vertexFirst = NULL;
    Vertex *vertexSecond = NULL;
    for (currentVertex = graph->allVertex; currentVertex != NULL; currentVertex = currentVertex->next)
    {
        if (currentVertex->vertex->ID == data[1][0])
        {

            vertexFirst = currentVertex->vertex;
        }
        if (currentVertex->vertex->ID == data[2][0])
        {
            vertexSecond = currentVertex->vertex;
        }
    }
    if (!vertexFirst && !vertexSecond)
    {
        printf("Unknown nodes %c %c\n", data[1][0], data[2][0]);
        return graph;
    }
    if (!vertexFirst)
    {
        printf("Unknown node %c\n", data[1][0]);
        return graph;
    }
    if (!vertexSecond)
    {
        printf("Unknown node %c\n", data[2][0]);
        return graph;
    }
    Edge *currentEdge = makeEdge(vertexFirst, vertexSecond, atoi(data[3]));
    struct EdgeNode *currentNode = malloc(sizeof(struct EdgeNode));
    currentNode->edge = currentEdge;
    currentNode->next = NULL;
    struct EdgeNode *outEdgeNode = malloc(sizeof(struct EdgeNode));
    outEdgeNode->edge = currentEdge;
    outEdgeNode->next = NULL;
    if (!vertexFirst->outEdge)
    {
        vertexFirst->outEdge = outEdgeNode;
    }
    else
    {
        struct EdgeNode *lastNode = vertexFirst->outEdge;
        while (lastNode->next)
        {
            lastNode = lastNode->next;
        }
        lastNode->next = outEdgeNode;
    }
    struct EdgeNode *inEdgeNode = malloc(sizeof(struct EdgeNode));
    inEdgeNode->edge = currentEdge;
    inEdgeNode->next = NULL;
    if (!vertexSecond->inputEdge)
    {
        vertexSecond->inputEdge = inEdgeNode;
    }
    else
    {
        struct EdgeNode *lastNode = vertexSecond->inputEdge;
        while (lastNode->next)
        {
            lastNode = lastNode->next;
        }
        lastNode->next = inEdgeNode;
    }

    if (!graph->allEdge)
    {
        graph->allEdge = currentNode;
    }
    else
    {
        struct EdgeNode *lastEdge = graph->allEdge;
        for (; lastEdge->next != NULL; lastEdge = lastEdge->next)
            ;
        lastEdge->next = currentNode;
    }
    return graph;
}

/*Удалет ребро из графа, как из списка всех рёбер так и между вершинами*/
struct Graph *removeEdge(char ID1, char ID2, struct Graph *graph)
{
    if (!graph)
    {
        printf("Unknown nodes %c %c\n", ID1, ID2);
        return NULL;
    }
    bool flagFirstVertex = false;
    bool flagSecondVertex = false;
    struct VertexNode *currentVertex = NULL;
    Vertex *vertexFirst = NULL;
    Vertex *vertexSecond = NULL;
    for (currentVertex = graph->allVertex; currentVertex != NULL; currentVertex = currentVertex->next)
    {
        if (currentVertex->vertex->ID == ID1)
        {
            flagFirstVertex = true;
            vertexFirst = currentVertex->vertex;
        }
        if (currentVertex->vertex->ID == ID2)
        {
            flagSecondVertex = true;
            vertexSecond = currentVertex->vertex;
        }
    }
    if (!flagFirstVertex && !flagSecondVertex)
    {
        printf("Unknown nodes %c %c\n", ID1, ID2);
        return graph;
    }
    if (!flagFirstVertex && flagSecondVertex)
    {
        printf("Unknown node %c\n", ID1);
        return graph;
    }
    if (flagFirstVertex && !flagSecondVertex)
    {
        printf("Unknown node %c\n", ID2);
        return graph;
    }
    Edge *toRemove = NULL;
    struct EdgeNode *current = graph->allEdge;
    while (current)
    {
        if ((current->edge->to == vertexSecond) && (current->edge->from == vertexFirst))
        {
            toRemove = current->edge;
            break;
        }
        current = current->next;
    }
    if (toRemove != NULL)
    {
        removeEdgeFromEdgeNode(&vertexFirst->outEdge, toRemove);
        removeEdgeFromEdgeNode(&vertexSecond->inputEdge, toRemove);
        removeEdgeFromEdgeNode(&graph->allEdge, toRemove);
        free(toRemove);
    }
    return graph;
}

/*Удаляет вершину из графа, так же удаляются все исходящие и входящие рёбра*/
struct Graph *removeNode(char id, struct Graph *graph)
{
    if (!graph)
    {
        printf("Unknown node %c\n", id);
        return NULL;
    }
    struct VertexNode *current = graph->allVertex;
    struct VertexNode *prev = NULL;
    Vertex *toRemoveVertex = NULL;
    while (current)
    {
        if (current->vertex->ID == id)
        {
            toRemoveVertex = current->vertex;
            break;
        }
        prev = current;
        current = current->next;
    }
    if (!toRemoveVertex)
    {
        printf("Unknown node %c\n", id);
        return graph;
    }
    struct EdgeNode *currentEdgeNode = NULL;

    while (toRemoveVertex->inputEdge)
    {
        Edge *toRemoveEdge = toRemoveVertex->inputEdge->edge;
        char fromID = toRemoveEdge->from->ID;
        char toID = toRemoveVertex->ID;
        graph = removeEdge(fromID, toID, graph);
    }
    while (toRemoveVertex->outEdge)
    {
        Edge *toRemoveEdge = toRemoveVertex->outEdge->edge;
        char fromID = toRemoveVertex->ID;
        char toID = toRemoveEdge->to->ID;
        graph = removeEdge(fromID, toID, graph);
    }
    if (prev)
    {
        prev->next = current->next;
    }
    else
    {
        graph->allVertex = current->next;
    }
    free(current->vertex);
    free(current);
    return graph;
}

/*Обход графа в глубину
vertex - текущая вершина для обхода
inUsed - массив, флаг отмечает что вершина сейчас в пути обхода
visited - массив, флаг отмечает уже посещённые вершины
order - сохраняем вершины, их ID*/
void DFS(Vertex *vertex, int *inUsed, int *visited, char *order, int *index)
{
    visited[vertex->ID] = 1;
    inUsed[vertex->ID] = 1;

    struct EdgeNode *currentEdge = vertex->outEdge;
    while (currentEdge != NULL)
    {
        Vertex *soon = currentEdge->edge->to;
        if (!visited[soon->ID])
        {
            DFS(soon, inUsed, visited, order, index);
        }
        else
        {
            if (inUsed[soon->ID])
            {

                printf("Found loop %c->%c ", vertex->ID, soon->ID);
            }
        }
        currentEdge = currentEdge->next;
    }
    inUsed[vertex->ID] = 0;
    order[(*index)++] = vertex->ID;
}

/*Функция нумерует вершины графа в обратном порядке, ищет циклы*/
void RpoNumbering(struct Graph *graph, char startID)
{
    if (!graph)
    {
        return;
    }

    int visited[256] = {0};
    int inUsed[256] = {0};
    char order[256];
    int index = 0;
    Vertex *startVertex = NULL;

    struct VertexNode *currentNode = graph->allVertex;
    while (currentNode != NULL)
    {
        if (currentNode->vertex->ID == startID)
        {
            startVertex = currentNode->vertex;
            break;
        }
        currentNode = currentNode->next;
    }
    DFS(startVertex, inUsed, visited, order, &index);
    currentNode = graph->allVertex;
    while (currentNode != NULL)
    {
        Vertex *vertex = currentNode->vertex;
        if (!visited[vertex->ID])
        {
            DFS(vertex, inUsed, visited, order, &index);
        }
        currentNode = currentNode->next;
    }
    for (int i = index - 1; i >= 0; i--)
    {
        printf("%c ", order[i]);
    }
    printf("\n");
}

/*Вывод графа красивый*/
void printGraph(struct Graph *graph)
{
    if (!graph)
    {
        printf("Graph is empty!\n");
        return;
    }

    printf("\n=== GRAPH STRUCTURE ===\n");

    // Выводим все вершины
    printf("Vertices:\n");
    struct VertexNode *vNode = graph->allVertex;
    while (vNode != NULL)
    {
        printf("  [%c]\n", vNode->vertex->ID);
        vNode = vNode->next;
    }

    // Выводим все рёбра
    printf("\nEdges:\n");
    struct EdgeNode *eNode = graph->allEdge;
    while (eNode != NULL)
    {
        printf("  %c --(%d)--> %c\n",
               eNode->edge->from->ID,
               eNode->edge->weight,
               eNode->edge->to->ID);
        eNode = eNode->next;
    }

    // Детальная информация по каждой вершине
    printf("\nDetailed vertex connections:\n");
    vNode = graph->allVertex;
    while (vNode != NULL)
    {
        Vertex *v = vNode->vertex;
        printf("Vertex %c:\n", v->ID);

        // Входящие рёбра
        printf("  Incoming edges: ");
        struct EdgeNode *inEdge = v->inputEdge;
        while (inEdge)
        {
            printf("%c(%d) ", inEdge->edge->from->ID, inEdge->edge->weight);
            inEdge = inEdge->next;
        }

        // Исходящие рёбра
        printf("\n  Outgoing edges: ");
        struct EdgeNode *outEdge = v->outEdge;
        while (outEdge)
        {
            printf("%c(%d) ", outEdge->edge->to->ID, outEdge->edge->weight);
            outEdge = outEdge->next;
        }
        printf("\n\n");

        vNode = vNode->next;
    }
}

/*Считает общее колмчество вершин в графе*/
int countAllVertexInGraph(struct Graph *graph)
{
    struct VertexNode *current = graph->allVertex;
    int count = 0;
    while (current)
    {
        count++;
        current = current->next;
    }
    return count;
}

/*Освобождаем память от графа*/
void freeGraph(struct Graph *graph)
{
    if (!graph)
        return;

    struct EdgeNode *eNode = graph->allEdge;
    while (eNode)
    {
        struct EdgeNode *temp = eNode;
        eNode = eNode->next;
        free(temp->edge);
        free(temp);
    }

    struct VertexNode *vNode = graph->allVertex;
    while (vNode)
    {
        struct VertexNode *temp = vNode;
        vNode = vNode->next;

        struct EdgeNode *in = temp->vertex->inputEdge;
        while (in)
        {
            struct EdgeNode *tempIn = in;
            in = in->next;
            free(tempIn);
        }

        struct EdgeNode *out = temp->vertex->outEdge;
        while (out)
        {
            struct EdgeNode *tempOut = out;
            out = out->next;
            free(tempOut);
        }

        free(temp->vertex);
        free(temp);
    }

    free(graph);
}
