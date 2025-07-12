#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Graph.h"

char **parserInput(const char *input, int *count);

void main()
{
    struct Graph *graph = NULL;
    char input[128];
    while (fgets(input, sizeof(input), stdin) != NULL)
    {

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
        {
            continue;
        }
        int tokenCount = 0;
        char **tokens = parserInput(input, &tokenCount);
        if (strcmp(tokens[0], "NODE") == 0)
        {
            graph = addNODE(tokens[1], graph);
        }
        if (strcmp(tokens[0], "EDGE") == 0)
        {
            graph = addEdge(tokens, graph);
        }
        if (strcmp(tokens[0], "REMOVE") == 0)
        {
            if (strcmp(tokens[1], "NODE") == 0)
            {
                graph = removeNode(tokens[2][0], graph);
            }
            else
            {
                graph = removeEdge(tokens[2][0], tokens[3][0], graph);
            }
        }
        if (strcmp(tokens[0], "RPO_NUMBERING") == 0)
        {
            RpoNumbering(graph, tokens[1][0]);
        }
        if (strcmp(tokens[0], "DIJKSTRA") == 0)
        {
            DIJKSTRA(graph, tokens[1][0]);
        }
        if (strcmp(tokens[0], "MAX") == 0 && strcmp(tokens[1], "FLOW") == 0)
        {
            maxFlow(graph, tokens[2][0], tokens[3][0]);
        }
        for (int i = 0; i < tokenCount; i++)
            free(tokens[i]);
        free(tokens);
    }
    freeGraph(graph);
}

char **parserInput(const char *input, int *tokenCount)
{
    if (!input)
        return NULL;
    char **tokens = malloc(4 * sizeof(char *));
    while (*input)
    {
        while (*input == ' ')
        {
            input++;
        }
        if (!*input)
        {
            break;
        }

        const char *start = input;
        while (*input && *input != ' ')
        {
            input++;
        }

        int lenght = input - start;
        tokens[*tokenCount] = malloc(lenght + 1);
        strncpy(tokens[*tokenCount], start, lenght);
        tokens[*tokenCount][lenght] = '\0';
        (*tokenCount)++;
    }
    return tokens;
}

// char **parserInput(const char *input, int *count);
//
// void main()
// {
//     struct Graph *graph = NULL;
//     while (true)
//     {
//         char input[128];
//         fgets(input, sizeof(input), stdin);
//         input[strcspn(input, "\n")] = '\0';
//         int tokenCount = 0;
//         if (strcmp(input, "EXIT") == 0)
//         {
//             break;
//         }
//         char **tokens = parserInput(input, &tokenCount);
//         if (strcmp(tokens[0], "NODE") == 0)
//         {
//             graph = addNODE(tokens[1], graph);
//         }
//         if (strcmp(tokens[0], "EDGE") == 0)
//         {
//             graph = addEdge(tokens, graph);
//         }
//         if (strcmp(tokens[0], "REMOVE") == 0)
//         {
//             if (strcmp(tokens[1], "NODE") == 0)
//             {
//                 graph = removeNode(tokens[2][0], graph);
//             }
//             else
//             {
//                 graph = removeEdge(tokens[2][0], tokens[3][0], graph);
//             }
//         }
//         if (strcmp(tokens[0], "RPO_NUMBERING") == 0)
//         {
//             RpoNumbering(graph, tokens[1][0]);
//         }
//         if (strcmp(tokens[0], "DIJKSTRA") == 0)
//         {
//             DIJKSTRA(graph, tokens[1][0]);
//         }
//         if (strcmp(tokens[0], "MAX") == 0 && strcmp(tokens[1], "FLOW") == 0)
//         {
//             maxFlow(graph, tokens[2][0], tokens[3][0]);
//         }
//
//
//     }
//     // printGraph(graph);
//     freeGraph(graph);
// }
//
// char **parserInput(const char *input, int *tokenCount)
// {
//     if (!input)
//         return NULL;
//     char **tokens = malloc(4 * sizeof(char *));
//     while (true)
//     {
//         while (*input == ' ')
//         {
//             input++;
//         }
//         if (!*input)
//         {
//             break;
//         }
//
//         const char *start = input;
//         while (*input && *input != ' ')
//         {
//             input++;
//         }
//
//         int lenght = input - start;
//         tokens[*tokenCount] = malloc(lenght + 1);
//         strncpy(tokens[*tokenCount], start, lenght);
//         tokens[*tokenCount][lenght] = '\0';
//         (*tokenCount)++;
//     }
//     return tokens;
// }
