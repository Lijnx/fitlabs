#include <stdlib.h>
#include <stdio.h>
#include "bitmatrix.h"
#include "stack.h"

#define MAX_VERTICES 2000
#define MAX_EDGES(VERTICES) (VERTICES * VERTICES + VERTICES) >> 1

typedef enum {WHITE, GRAY, BLACK} Colour;

int read_edges(Bitmatrix *bm) {
    int strings = 0;
    while (!feof(stdin)) {
        int v0, v1;
        if (scanf("%d %d", &v0, &v1) == 2) {
            if (v0 >= 1 && v0 <= bm->size && v1 >= 1 && v1 <= bm->size)
                set_at(bm, v0 - 1, v1 - 1, 1);
            else return -1;
            strings++;
        }
    }
    return strings;
}

int dfs(Bitmatrix adj_arr, Stack *order, Colour *visited, int at) {
    if (visited[at] == WHITE) {
        visited[at] = GRAY;
        for (int i = 0; i < adj_arr.size; i++) {
            if (get_at(adj_arr, at, i)) {
                if (dfs(adj_arr, order, visited, i))
                    return 1;
            }
        }
        visited[at] = BLACK;
        push(order, at + 1);
        return 0;
    }
    else if (visited[at] == GRAY)
        return 1;
    else
        return 0;
}

Stack top_sort(Bitmatrix adj_arr) {
    Stack order;
    init_stack(&order);
    Colour *visited = (Colour*)calloc(adj_arr.size, sizeof(Colour));
    if (visited) {
        for (int i = 0; i < adj_arr.size; i++) {
            if (dfs(adj_arr, &order, visited, i)) {
                free(visited);
                delete_stack(&order);
                return order;
            }
        }
        free(visited);
    }
    return order;
}

int main(void) {

    int vertices;
    if (scanf("%d", &vertices) == 1) {
        if (vertices >= 0 && vertices <= MAX_VERTICES) {
            int edges;
            if (scanf("%d", &edges) == 1) {
                if (edges >= 0 && edges <= MAX_EDGES(vertices)) {
                    Bitmatrix adj_arr;
                    if (init_matrix(&adj_arr, vertices)) {
                        int strings = read_edges(&adj_arr);
                        if (strings >= edges) {
                            Stack order = top_sort(adj_arr);
                            if (!is_empty(order)) {
                                print_stack(order);
                                delete_stack(&order);
                            }
                            else puts("impossible to sort");

                        }
                        else if (strings == -1)
                            puts("bad vertex");
                        else
                            puts("bad number of lines");
                        delete_matrix(&adj_arr);
                    }
                }
                else puts("bad number of edges");
            }
            else puts("bad number of lines");
        }
        else puts("bad number of vertices");
    }
    else puts("bad number of lines");

    return EXIT_SUCCESS;
}
