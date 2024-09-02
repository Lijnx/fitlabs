#include "graph.h"

Pair make_pair(int x, int y) {
    Pair pair;
    pair.x = x;
    pair.y = y;
    return pair;
}

Graph init_graph() {
    Graph graph;
    graph.vertices_number = 0;
    graph.edges_number = 0;
    graph.edges = NULL;
    return graph;
}

Edge create_edge(int weight, int v1, int v2) {
    Edge edge;
    edge.weight = weight;
    edge.vertices = make_pair(v1, v2);
    return edge;
}

int read_edges(Graph *graph, FILE *input) {
    //skip '\n' symbol
    fgetc(input);
    for (int i = 0; i < graph->edges_number; i++) {
        char buffer[BUFFER_SIZE];
        if (fgets(buffer, BUFFER_SIZE, input) != NULL) {
            char *ptr = buffer;
            long long v1, v2, w;
            v1 = strtoll(ptr, &ptr, 10);
            v2 = strtoll(ptr, &ptr, 10);
            w = strtoll(ptr, &ptr, 10);

            if (v1 >= 1 && v1 <= graph->vertices_number && v2 >= 1 && v2 <= graph->vertices_number) {
                if (w >= 0 && w <= INT_MAX)
                    graph->edges[i] = create_edge((int)w, (int)(v1 - 1), (int)(v2 - 1));
                else return BAD_LENGTH;
            }
            else return BAD_VERTEX;
        }
        else return BAD_LINES_NUMBER;
    }
    return 0;
}

int read_graph(Graph *graph, FILE *input) {
    if (fscanf(input, "%d", &graph->vertices_number) == 1) {
        if (graph->vertices_number >= 0 && graph->vertices_number <= MAX_VERTICES) {
            if (fscanf(input, "%d", &graph->edges_number) == 1) {
                if (graph->edges_number >= 0 && graph->edges_number <= MAX_EDGES(graph->vertices_number)) {
                    if (graph->edges_number != 0) {
                        graph->edges = (Edge*)malloc(graph->edges_number * sizeof(Edge));
                        if (graph->edges)
                            return read_edges(graph, input);
                        else fputs("Graph::Stack Overflow\n", stderr);
                    }
                    else graph->edges = NULL;
                }
                else return BAD_EDGES_NUMBER;
            }
            else return BAD_LINES_NUMBER;
        }
        else return BAD_VERTICES_NUMBER;
    }
    else return BAD_LINES_NUMBER;

    return 0;
}

void input_errors_handler(int err) {
    switch (err) {
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
        case NO_SPANNING_TREE:
            puts("no spanning tree");
            break;
        default:
            break;
    }
}

int compare(const void* edge1, const void* edge2) {
    return (*(Edge*)edge1).weight - (*(Edge*)edge2).weight;
}

void print_array(Pair* array, int size) {
    if (array) {
        for (int i = 0; i < size; i++)
            printf("%d %d\n", array[i].x, array[i].y);
    }
}

void kruskal(Graph graph) {
    qsort(graph.edges, graph.edges_number, sizeof(Edge), compare);
    DSU dsu;
    if (init_dsu(&dsu, graph.vertices_number)) {
        for (int i = 0; i < graph.vertices_number; i++)
            make_set(&dsu, i);

        Pair *result = NULL;
        if (graph.vertices_number > 1) {
            result = (Pair*)malloc((graph.vertices_number - 1) * sizeof(Pair));
            if (result) {
                int k = 0;
                for (int i = 0; i < graph.edges_number; i++) {
                    int x = graph.edges[i].vertices.x, y = graph.edges[i].vertices.y;
                    if (find_set(&dsu, x) != find_set(&dsu, y)) {
                        result[k++] = make_pair(x + 1, y + 1);
                        union_sets(&dsu, x, y);
                    }
                }
            }
            else fputs("Graph::Stack Overflow\n", stderr);
        }

        if (is_whole(&dsu, graph.vertices_number))
            print_array(result, graph.vertices_number - 1);
        else puts("no spanning tree");

        free(result);
        delete_dsu(&dsu);
    }
}

void delete_graph(Graph *graph) {
    free(graph->edges);
    graph->edges = NULL;
    graph->edges_number = 0;
    graph->vertices_number = 0;
}
