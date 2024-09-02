#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int *parent;
    int *size;
} DSU;

int init_dsu(DSU *dsu, int size);
void make_set(DSU *dsu, int x);
int find_set(DSU *dsu, int x);
void union_sets(DSU *dsu, int x, int y);
int is_whole(DSU *dsu, int size);
void delete_dsu(DSU *dsu);
