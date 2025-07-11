#include "Graph.h"

Vertex *makeVertex(char id)
{
    Vertex *vertex = malloc(sizeof(Vertex));
    vertex->ID = id;
    vertex->inputEdge = NULL;
    vertex->outEdge = NULL;
    return vertex;
}
Edge *makeEdge(Vertex *vertexFirst, Vertex *vertexSecond, int weight)
{
    Edge *edge = malloc(sizeof(Edge));
    edge->from = vertexFirst;
    edge->to = vertexSecond;
    edge->weight = weight;
    return edge;
}

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
    //////
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
    if (startVertex == NULL)
    {
        printf("Start vertex not found.\n");
        return;
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

struct Graph *cloneGraph(struct Graph *orig)
{
    if (!orig)
        return NULL;

    // Создаем новый граф
    struct Graph *clone = malloc(sizeof(struct Graph));
    clone->allVertex = NULL;
    clone->allEdge = NULL;

    // Создаем маппинг оригинальных вершин на их копии
    struct VertexNode *vNode = orig->allVertex;
    struct VertexNode **cloneVNode = &clone->allVertex;

    // Первый проход: копируем все вершины
    while (vNode)
    {
        // Создаем копию вершины
        Vertex *newVertex = makeVertex(vNode->vertex->ID);

        // Добавляем вершину в список вершин клона
        *cloneVNode = malloc(sizeof(struct VertexNode));
        (*cloneVNode)->vertex = newVertex;
        (*cloneVNode)->next = NULL;

        // Переходим к следующей вершине
        cloneVNode = &(*cloneVNode)->next;
        vNode = vNode->next;
    }

    // Второй проход: копируем все ребра
    struct EdgeNode *eNode = orig->allEdge;
    struct EdgeNode **cloneENode = &clone->allEdge;

    while (eNode)
    {
        // Находим копии вершин from и to в клоне
        Vertex *originalFrom = eNode->edge->from;
        Vertex *originalTo = eNode->edge->to;

        Vertex *cloneFrom = NULL;
        Vertex *cloneTo = NULL;

        // Ищем соответствующие вершины в клоне
        struct VertexNode *vNodeOrig = orig->allVertex;
        struct VertexNode *vNodeClone = clone->allVertex;

        while (vNodeOrig && vNodeClone)
        {
            if (vNodeOrig->vertex == originalFrom)
            {
                cloneFrom = vNodeClone->vertex;
            }
            if (vNodeOrig->vertex == originalTo)
            {
                cloneTo = vNodeClone->vertex;
            }
            vNodeOrig = vNodeOrig->next;
            vNodeClone = vNodeClone->next;
        }

        if (cloneFrom && cloneTo)
        {
            // Создаем новое ребро
            Edge *newEdge = makeEdge(cloneFrom, cloneTo, eNode->edge->weight);

            // Добавляем ребро в список ребер графа
            *cloneENode = malloc(sizeof(struct EdgeNode));
            (*cloneENode)->edge = newEdge;
            (*cloneENode)->next = NULL;
            cloneENode = &(*cloneENode)->next;

            // Добавляем ребро в исходящие ребра from-вершины
            struct EdgeNode *outEdgeNode = malloc(sizeof(struct EdgeNode));
            outEdgeNode->edge = newEdge;
            outEdgeNode->next = NULL;

            if (!cloneFrom->outEdge)
            {
                cloneFrom->outEdge = outEdgeNode;
            }
            else
            {
                struct EdgeNode *last = cloneFrom->outEdge;
                while (last->next)
                    last = last->next;
                last->next = outEdgeNode;
            }

            // Добавляем ребро в входящие ребра to-вершины
            struct EdgeNode *inEdgeNode = malloc(sizeof(struct EdgeNode));
            inEdgeNode->edge = newEdge;
            inEdgeNode->next = NULL;

            if (!cloneTo->inputEdge)
            {
                cloneTo->inputEdge = inEdgeNode;
            }
            else
            {
                struct EdgeNode *last = cloneTo->inputEdge;
                while (last->next)
                    last = last->next;
                last->next = inEdgeNode;
            }
        }

        eNode = eNode->next;
    }

    return clone;
}

void addEdgePath(struct EdgeNode **path, Edge *edge)
{
    struct EdgeNode *new = malloc(sizeof(struct EdgeNode));
    new->edge = edge;
    new->next = *path;
    *path = new;
}

void removeEdgePath(struct EdgeNode **path)
{
    if (*path)
    {
        struct EdgeNode *top = *path;
        *path = top->next;
        free(top);
    }
}

struct EdgeNode *findPathDFS(Vertex *current, Vertex *target, bool *visited, struct EdgeNode **currentPath)
{
    if (current == target)
    {
        struct EdgeNode *result = NULL;
        struct EdgeNode *tmp = *currentPath;

        while (tmp)
        {
            struct EdgeNode *new = malloc(sizeof(struct EdgeNode));
            new->edge = tmp->edge;
            new->next = result;
            result = new;
            tmp = tmp->next;
        }
        return result;
    }

    visited[current->ID] = true;
    struct EdgeNode *result = NULL;

    for (struct EdgeNode *now = current->outEdge; now && !result; now = now->next)
    {
        Vertex *next = now->edge->to;

        if (!visited[next->ID])
        {
            addEdgePath(currentPath, now->edge);
            result = findPathDFS(next, target, visited, currentPath);
            removeEdgePath(currentPath);
        }
    }
    visited[current->ID] = false;
    return result;
}
struct EdgeNode *findPath(struct Graph *graph, Vertex *start, Vertex *end)
{
    if (start == end)
        return NULL;

    bool visited[256] = {false};
    struct EdgeNode *currentPath = NULL;
    struct EdgeNode *path = findPathDFS(start, end, visited, &currentPath);
    // struct EdgeNode *reverse = NULL;
    // while (path)
    // {
    //     struct EdgeNode *next = path->next;
    //     path->next = reverse;
    //     reverse = path;
    //     path = next;
    // }
    return path;
}
void printPath(struct EdgeNode *path)
{
    if (!path)
    {
        printf("Путь не найден\n");
        return;
    }

    struct EdgeNode *current = path;

    // Печатаем первую вершину
    printf("%c", current->edge->from->ID);

    while (current)
    {
        // Печатаем ребро и следующую вершину
        printf("-(%d)->%c", current->edge->weight, current->edge->to->ID);
        current = current->next;
    }
    printf("\n");
}

void maxFlow(struct Graph *graph, char startID, char endID)
{
    struct Graph *clone = cloneGraph(graph);
    struct VertexNode *now = clone->allVertex;
    Vertex *vertexStart = NULL;
    Vertex *vertexEnd = NULL;
    while (now)
    {
        if (now->vertex->ID == startID)
            vertexStart = now->vertex;
        if (now->vertex->ID == endID)
            vertexEnd = now->vertex;
        now = now->next;
    }

    printPath(findPath(clone, vertexStart, vertexEnd));
}
