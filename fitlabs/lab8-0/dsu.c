#include "dsu.h"

void swap(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

int init_dsu(DSU *dsu, int size) {
    if (size != 0) {
        dsu->parent = (int*)malloc(size * sizeof(int));
        dsu->size = (int*)malloc(size * sizeof(int));
        if (dsu->parent && dsu->size)
            return 1;
        else {
            fputs("DSU::Stack Overflow\n", stderr);
            return 0;
        }
    }
    else {
        dsu->parent = NULL;
        dsu->size = NULL;
        return 1;
    }
}

void make_set(DSU *dsu, int x) {
    dsu->parent[x] = x;
    dsu->size[x] = 1;
}

int find_set(DSU *dsu, int x) {
    if (dsu->parent[x] == x)
        return x;
    return dsu->parent[x] = find_set(dsu, dsu->parent[x]);
}

void union_sets (DSU *dsu, int x, int y) {
    x = find_set (dsu, x);
    y = find_set (dsu, y);
    if (x != y) {
        if (dsu->size[x] < dsu->size[y])
            swap (&x, &y);
        dsu->parent[y] = x;
        dsu->size[x] += dsu->size[y];
    }
}

int is_whole(DSU *dsu, int size) {
    if (dsu->parent && dsu->size)
        return (dsu->size[find_set(dsu, 0)] == size) ? 1 : 0;
    else return 0;
}

void delete_dsu(DSU *dsu) {
    free(dsu->parent);
    free(dsu->size);
    dsu->parent = NULL;
    dsu->size = NULL;
}
