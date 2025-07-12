#include "Graph.h"
/*Клонируем граф, делаем копию*/
struct Graph *cloneGraph(struct Graph *orig)
{
    if (!orig)
        return NULL;

    struct Graph *clone = malloc(sizeof(struct Graph));
    clone->allVertex = NULL;
    clone->allEdge = NULL;
    struct VertexNode *vNode = orig->allVertex;
    struct VertexNode **cloneVNode = &clone->allVertex;
    while (vNode)
    {

        Vertex *newVertex = makeVertex(vNode->vertex->ID);

        *cloneVNode = malloc(sizeof(struct VertexNode));
        (*cloneVNode)->vertex = newVertex;
        (*cloneVNode)->next = NULL;

        cloneVNode = &(*cloneVNode)->next;
        vNode = vNode->next;
    }
    struct EdgeNode *eNode = orig->allEdge;
    struct EdgeNode **cloneENode = &clone->allEdge;
    while (eNode)
    {
        Vertex *originalFrom = eNode->edge->from;
        Vertex *originalTo = eNode->edge->to;
        Vertex *cloneFrom = NULL;
        Vertex *cloneTo = NULL;
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

            Edge *newEdge = makeEdge(cloneFrom, cloneTo, eNode->edge->weight);

            *cloneENode = malloc(sizeof(struct EdgeNode));
            (*cloneENode)->edge = newEdge;
            (*cloneENode)->next = NULL;
            cloneENode = &(*cloneENode)->next;

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

/*Добавляем ребро в ПУТЬ*/
void addEdgePath(struct EdgeNode **path, Edge *edge)
{
    struct EdgeNode *new = malloc(sizeof(struct EdgeNode));
    new->edge = edge;
    new->next = *path;
    *path = new;
}
/*Удаляем ребро из ПУТИ*/
void removeEdgePath(struct EdgeNode **path)
{
    if (*path)
    {
        struct EdgeNode *top = *path;
        *path = top->next;
        free(top);
    }
}

/*Непоследственно ищет путь ЛЮБОЙ между вершинами используя DFS
current - текущая вершина
target - вершина сток
visited - массив отслеживаем посещённые вершины
currentPath - строим путь*/
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
/*Ищем путь от истока в сток
start - исток
end - сток*/
struct EdgeNode *findPath(struct Graph *graph, Vertex *start, Vertex *end)
{
    if (start == end)
        return NULL;

    bool visited[256] = {false};
    struct EdgeNode *currentPath = NULL;
    struct EdgeNode *path = findPathDFS(start, end, visited, &currentPath);
    return path;
}
void printPath(struct EdgeNode *path)
{
    if (!path)
    {
        return;
    }

    struct EdgeNode *current = path;

    printf("%c", current->edge->from->ID);

    while (current)
    {

        printf("-(%d)->%c", current->edge->weight, current->edge->to->ID);
        current = current->next;
    }
    printf("\n");
}

/*Ищем минимальное "узкое" место в пути*/
int findMinCap(struct EdgeNode *path)
{
    if (!path)
        return 0;
    int min = 32000;
    struct EdgeNode *current = path;

    while (current)
    {
        if (current->edge->weight < min)
        {
            min = current->edge->weight;
        }
        current = current->next;
    }
    return min;
}

/*Добавляем обратное ребро*/
void addReverseEdge(struct Graph *graph, Vertex *from, Vertex *to, int weight)
{
    struct EdgeNode *eNode = from->outEdge;
    while (eNode)
    {
        if (eNode->edge->to == to)
        {
            eNode->edge->weight += weight;
            return;
        }
        eNode = eNode->next;
    }

    Edge *reverseEdge = makeEdge(from, to, weight);

    struct EdgeNode *newEdgeNode = malloc(sizeof(struct EdgeNode));
    newEdgeNode->edge = reverseEdge;
    newEdgeNode->next = graph->allEdge;
    graph->allEdge = newEdgeNode;

    newEdgeNode = malloc(sizeof(struct EdgeNode));
    newEdgeNode->edge = reverseEdge;
    newEdgeNode->next = from->outEdge;
    from->outEdge = newEdgeNode;

    newEdgeNode = malloc(sizeof(struct EdgeNode));
    newEdgeNode->edge = reverseEdge;
    newEdgeNode->next = to->inputEdge;
    to->inputEdge = newEdgeNode;
}
/*По сути лишняя функция можно было использовать уже реализованные*/
void removeEdgeFromGraph(struct Graph *graph, Edge *edge)
{
    removeEdgeFromEdgeNode(&graph->allEdge, edge);
    removeEdgeFromEdgeNode(&edge->from->outEdge, edge);
    removeEdgeFromEdgeNode(&edge->to->inputEdge, edge);
    free(edge);
}

/*Обновляем граф после нахождения пути от истока в сток*/
void updateEdgeCloneGraph(struct Graph *graph, struct EdgeNode *path, int flow)
{
    struct EdgeNode *current = path;

    while (current)
    {
        Edge *edge = current->edge;
        Vertex *fromVertex = edge->from;
        Vertex *toVertex = edge->to;

        edge->weight -= flow;
        if (edge->weight <= 0)
        {
            removeEdgeFromGraph(graph, edge);
        }

        addReverseEdge(graph, toVertex, fromVertex, flow);

        current = current->next;
    }
}

void freePath(struct EdgeNode *path)
{
    while (path)
    {
        struct EdgeNode *current = path;
        path = path->next;
        free(current);
    }
}

/*Ищем максимальный поток от истока в сток*/
void maxFlow(struct Graph *graph, char startID, char endID)
{
    struct Graph *clone = cloneGraph(graph);
    struct VertexNode *now = clone->allVertex;
    Vertex *vertexStart = NULL;
    Vertex *vertexEnd = NULL;
    int maxFlow = 0;
    while (now)
    {
        if (now->vertex->ID == startID)
            vertexStart = now->vertex;
        if (now->vertex->ID == endID)
            vertexEnd = now->vertex;
        now = now->next;
    }

    struct EdgeNode *path;
    while ((path = findPath(clone, vertexStart, vertexEnd)) != NULL)
    {
        int flow = findMinCap(path);
        updateEdgeCloneGraph(clone, path, flow);

        maxFlow += flow;
        freePath(path);
    }

    printf("%d\n", maxFlow);
    freeGraph(clone);
}
