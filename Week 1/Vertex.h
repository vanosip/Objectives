/*Узел(вершина)
ID - идентификатор(метка)
inputEdge - входящие рёбра список
outEdge - исходящие рёбра список
*/
typedef struct Vertex
{
    char ID;
    struct EdgeNode *inputEdge;
    struct EdgeNode *outEdge;

} Vertex;

/*Список вершин(узлов) вспомогательная структура*/
struct VertexNode
{
    Vertex *vertex;
    struct VertexNode *next;
};
