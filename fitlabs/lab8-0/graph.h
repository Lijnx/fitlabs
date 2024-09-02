#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "dsu.h"

#define BUFFER_SIZE 255
#define MAX_VERTICES 5000
#define MAX_EDGES(VERTICES) ((VERTICES * VERTICES - VERTICES) >> 1)

#define BAD_VERTICES_NUMBER -1
#define BAD_EDGES_NUMBER -2
#define BAD_LINES_NUMBER -3
#define BAD_VERTEX -4
#define BAD_LENGTH -5
#define NO_SPANNING_TREE -6

typedef struct {
    int x;
    int y;
} Pair;

typedef struct {
    int weight;
    Pair vertices;
} Edge;

typedef struct {
    int vertices_number;
    Edge *edges;
    int edges_number;
} Graph;

Pair make_pair(int x, int y);
Graph init_graph();
Edge create_edge(int weight, int v1, int v2);
int read_graph(Graph *graph, FILE *input);
void input_errors_handler(int err);
void kruskal(Graph graph);
void delete_graph(Graph *graph);
