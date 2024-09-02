#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

int main(void) {

    FILE *input = fopen("in.txt", "r");
    if (input == NULL)
        return EXIT_SUCCESS;

    Graph graph = init_graph();
    if (fill_graph(&graph, input) != NULL) {
        prim(graph);
        free_graph(&graph);
    }
    fclose(input);

    return EXIT_SUCCESS;
}
