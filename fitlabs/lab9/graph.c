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
    if (fscanf(input, "%d %d", &graph->from, &graph->to) != 2)
        return BAD_LINES_NUMBER;
    if (graph->from < 1 || graph->from > graph->verts_num || graph->to < 1 || graph->to > graph->verts_num)
        return BAD_VERTEX;
    graph->from--;
    graph->to--;
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
    graph.from = 0;
    graph.to = 0;
    graph.matrix = NULL;
    return graph;
}

void free_graph(Graph* graph) {
    if (graph->matrix != NULL) {
        for (int i = 0; i < graph->verts_num; i++)
            free(graph->matrix[i]);
        free(graph->matrix);
        graph->matrix = NULL;
    }
    graph->verts_num = 0;
    graph->edges_num = 0;
    graph->from = 0;
    graph->to = 0;
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

void print_distances(Graph graph, long long* distances) {
    for (int i = 0; i < graph.verts_num; i++) {
        if (distances[i] == LLONG_MAX)
            printf("oo ");
        else if (distances[i] > INT_MAX)
            printf("INT_MAX+ ");
        else
            printf("%lld ", distances[i]);
    }
    printf("\n");
}

int check_paths(Graph graph, const int* precursor, const long long* distances) {
    if (distances[graph.to] == LLONG_MAX) {
        printf("no path\n");
        return 0;
    }

    if (distances[graph.to] <= INT_MAX)
        return 1;

    int greater_intmax = 0;
    for (int i = 0; i < graph.verts_num; i++) {
        if (graph.matrix[graph.to][i] != -1 && precursor[i] != graph.to &&
                distances[i] + graph.matrix[graph.to][i] > INT_MAX) {

            greater_intmax++;
        }
    }
    if (greater_intmax >= 2) {
        printf("overflow\n");
        return 0;
    }

    return 1;
}

void print_path(Graph graph, const int* precursor) {
    printf("%d ", graph.to + 1);
    int current = graph.to;
    while (precursor[current] != graph.from) {
        current = precursor[current];
        printf("%d ", current + 1);
    }
    printf("%d\n", graph.from + 1);
}

void dijkstra(Graph graph) {
    if (is_empty(graph))
        return;

    Fibheap heap = fb_init();
    Node** map = (Node**)malloc(sizeof(Node*) * graph.verts_num);
    long long *distances = (long long*)malloc(sizeof(long long) * graph.verts_num);
    int *precursor = (int*)malloc(sizeof(int) * graph.verts_num);

    if (map == NULL || distances == NULL || precursor == NULL) {
        free(map);
        free(distances);
        free(precursor);
        return;
    }

    for (int i = 0; i < graph.verts_num; i++) {
        map[i] = fb_insert(&heap, i, LLONG_MAX);
        distances[i] = LLONG_MAX;
    }

    fb_decrease_key(&heap, map[graph.from], 0);
    distances[graph.from] = 0;
    precursor[graph.from] = graph.from;

    for (int i = 0; i < graph.verts_num; i++) {

        Node* min = fb_extract_min(&heap);
        map[min->value] = NULL;

        for (int j = 0; j < graph.verts_num; j++) {
            if (graph.matrix[min->value][j] != -1 && map[j] != NULL &&
                    distances[min->value] + graph.matrix[min->value][j] < distances[j]) {

                precursor[j] = min->value;
                distances[j] = distances[min->value] + graph.matrix[min->value][j];
                fb_decrease_key(&heap, map[j], distances[j]);
            }
        }

        free(min);
    }
    free(map);

    print_distances(graph, distances);

    if (check_paths(graph, precursor, distances))
        print_path(graph, precursor);

    free(distances);
    free(precursor);
}
