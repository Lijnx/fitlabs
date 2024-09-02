#include "graph.h"

Graph* init_matrix(Graph* graph) {
    if (graph->verts_num == 0) {
        graph->matrix = NULL;
        return graph;
    }

    graph->matrix = (int**)malloc(sizeof(int*) * graph->verts_num);
    if (graph->matrix == NULL)
        return NULL;

    for (int i = 0; i < graph->verts_num; i++) {
        graph->matrix[i] = (int*)malloc(sizeof(int) * graph->verts_num);
        if (graph->matrix[i] == NULL) {
            for (int j = 0; j < i; j++)
                free(graph->matrix[j]);
            free(graph->matrix);
            return NULL;
        }
    }

    for (int i = 0; i < graph->verts_num; i++)
        for (int j = 0; j < graph->verts_num; j++)
            graph->matrix[i][j] = -1;

    return graph;
}

int read_edges(Graph* graph, FILE *input) {
    //skip '\n' symbol
    fgetc(input);
    for (int i = 0; i < graph->edges_num; i++) {
        char buffer[BUFFER_SIZE];

        if (fgets(buffer, BUFFER_SIZE, input) == NULL)
            return BAD_LINES_NUMBER;

        char *ptr = buffer;
        long long v1, v2, w;
        v1 = strtoll(ptr, &ptr, 10);
        v2 = strtoll(ptr, &ptr, 10);
        w = strtoll(ptr, &ptr, 10);

        if (v1 < 1 || v1 > graph->verts_num || v2 < 1 || v2 > graph->verts_num)
            return BAD_VERTEX;
        if (w < 0 || w > INT_MAX)
            return BAD_LENGTH;

        graph->matrix[v1 - 1][v2 - 1] = (int)w;
        graph->matrix[v2 - 1][v1 - 1] = (int)w;
    }
    return 0;
}

int read_graph(Graph* graph, FILE *input) {
    if (fscanf(input, "%d", &graph->verts_num) != 1)
        return BAD_LINES_NUMBER;
    if (graph->verts_num < 0 || graph->verts_num > MAX_VERTICES)
        return BAD_VERTICES_NUMBER;
    if (fscanf(input, "%d", &graph->edges_num) != 1)
        return BAD_LINES_NUMBER;
    if (graph->edges_num < 0 || graph->edges_num > MAX_EDGES(graph->verts_num))
        return BAD_EDGES_NUMBER;

    if (init_matrix(graph) == NULL)
        return MEMORY_ERROR;

    return read_edges(graph, input);
}

void errors_handler(int err) {
    switch (err) {
        case MEMORY_ERROR:
            puts("memory error");
            break;
        case BAD_VERTICES_NUMBER:
            puts("bad number of vertices");
            break;
        case BAD_EDGES_NUMBER:
            puts("bad number of edges");
            break;
        case BAD_LINES_NUMBER:
            puts("bad number of lines");
            break;
        case BAD_VERTEX:
            puts("bad vertex");
            break;
        case BAD_LENGTH:
            puts("bad length");
            break;
        default:
            break;
    }
}

int is_empty(Graph graph) {
    return (graph.verts_num == 0) ? 1 : 0;
}

Graph init_graph() {
    Graph graph;
    graph.verts_num = 0;
    graph.edges_num = 0;
    graph.matrix = NULL;
    return graph;
}

Graph* fill_graph(Graph* graph, FILE* input) {
    int err = read_graph(graph, input);
    errors_handler(err);
    if (err == 0)
        return graph;
    else {
        free_graph(graph);
        return NULL;
    }
}

void free_graph(Graph* graph) {
    if (graph->matrix != NULL) {
        for (int i = 0; i < graph->verts_num; i++)
            free(graph->matrix[i]);
        free(graph->matrix);
    }
    graph->verts_num = 0;
    graph->edges_num = 0;
}

void prim(Graph graph) {
    if (is_empty(graph)) {
        puts("no spanning tree");
        return;
    }

    Fibheap heap = fb_init();
    Node** map = (Node**)malloc(sizeof(Node*) * graph.verts_num);
    int* precursor = (int*)malloc(sizeof(int) * graph.verts_num);
    Pair* result = (Pair*)malloc(sizeof(Pair) * (graph.verts_num - 1));

    if (map == NULL || precursor == NULL || result == NULL) {
        free(map);
        free(precursor);
        free(result);
        return;
    }

    for (int i = 0; i < graph.verts_num; i++) {
        map[i] = fb_insert(&heap, i, LLONG_MAX);
        precursor[i] = -1;
    }

    int k = 0;
    for (int i = 0; i < graph.verts_num; i++) {

        Node* min = fb_extract_min(&heap);
        map[min->value] = NULL;

        for (int j = 0; j < graph.verts_num; j++) {
            if (graph.matrix[min->value][j] != -1 && map[j] != NULL && (long)graph.matrix[min->value][j] < map[j]->key) {
                fb_decrease_key(&heap, map[j], graph.matrix[min->value][j]);
                precursor[j] = min->value;
            }
        }

        if (precursor[min->value] != -1) {
            result[k].first = precursor[min->value];
            result[k].second = min->value;
            k++;
        }

        free(min);
    }
    free(map);
    free(precursor);

    if (k != graph.verts_num - 1) {
        puts("no spanning tree");
        free(result);
        return;
    }

    for (int i = 0; i < k; i++) {
        printf("%d %d\n", result[i].first + 1, result[i].second + 1);
    }
    free(result);
}
