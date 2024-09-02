#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    unsigned char **matrix;
    int size;
} Bitmatrix;

int init_matrix(Bitmatrix *bm, int size);
int set_at(Bitmatrix *bm, int i, int j, unsigned char bit);
int get_at(Bitmatrix bm, int i, int j);
void delete_matrix(Bitmatrix *bm);
