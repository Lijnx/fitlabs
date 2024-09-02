#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "fibheap.h"

#define BUFFER_SIZE 255
#define MAX_VERTICES 5000
#define MAX_EDGES(VERTICES) ((VERTICES * VERTICES - VERTICES) >> 1)

#define MEMORY_ERROR -1
#define BAD_VERTICES_NUMBER -2
#define BAD_EDGES_NUMBER -3
#define BAD_LINES_NUMBER -4
#define BAD_VERTEX -5
#define BAD_LENGTH -6

typedef struct {
    int first;
    int second;
} Pair;

typedef struct {
    int **matrix;
    int verts_num;
    int edges_num;
} Graph;

int is_empty(Graph graph);
Graph init_graph();
Graph* fill_graph(Graph* graph, FILE* input);
void free_graph(Graph* graph);
void prim(Graph graph);
