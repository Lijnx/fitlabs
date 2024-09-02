#include <stdlib.h>
#include "graph.h"

//Просто проклятая лаба

int main(void) {

    FILE *input = fopen("in.txt", "r");
    if (input) {
        Graph graph = init_graph();
        int err = read_graph(&graph, input);

        if (err != 0)
            input_errors_handler(err);
        else
            kruskal(graph);

        delete_graph(&graph);
    }

    return EXIT_SUCCESS;
}
