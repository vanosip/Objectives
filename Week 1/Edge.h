#include "Vertex.h"

/*Ребро
weight - вес целое неотрицательное число
from- узел-предшественник
to - узел-преемник
*/
typedef struct Edge
{
    int weight;
    Vertex *from;
    Vertex *to;
} Edge;
/*Список ребёр вспомогательная структура*/
struct EdgeNode
{
    struct Edge *edge;
    struct EdgeNode *next;
};