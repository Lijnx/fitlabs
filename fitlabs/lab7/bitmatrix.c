#include "bitmatrix.h"

#define MOD8(n) (n - ((n >> 3) << 3))
#define BYTE_SIZE(bits) ((MOD8(bits)) ? (bits >> 3) + 1 : bits >> 3)

int init_matrix(Bitmatrix *bm, int size) {
    if (size > 0) {
        bm->matrix = (unsigned char**)malloc(size * sizeof(char*));
        if (bm->matrix) {
            int byte_size = BYTE_SIZE(size);
            for (int i = 0; i < size; i++) {
                bm->matrix[i] = (unsigned char*)calloc(byte_size, sizeof(char));
                if (!bm->matrix[i]) {
                    for (int j = 0; j < i; j++)
                        free(bm->matrix[i]);
                    free(bm->matrix);
                    bm->matrix = NULL;
                    bm->size = 0;
                    fputs("Bitmatrix::Stack Overflow\n", stderr);
                    return 0;
                }
            }
            bm->size = size;
            return 1;
        }
        else {
            bm->matrix = NULL;
            bm->size = 0;
            fputs("Bitmatrix::Stack Overflow\n", stderr);
            return 0;
        }
    }
    else {
        bm->matrix = NULL;
        bm->size = 0;
        return 1;
    }
}

int set_at(Bitmatrix *bm, int i, int j, unsigned char bit) {
    if (bm->matrix && i < bm->size && j < bm->size) {
        int j_offset = 7 - MOD8(j);
        j = j >> 3;
        if (bit)
            bm->matrix[i][j] |= (1 << j_offset);
        else
            bm->matrix[i][j] &= ~(1 << j_offset);
        return 1;
    }
    else return 0;
}

int get_at(Bitmatrix bm, int i, int j) {
    if (bm.matrix && i < bm.size && j < bm.size) {
        int j_offset = 7 - MOD8(j);
        j = j >> 3;
        return (bm.matrix[i][j] >> j_offset) & 1;
    }
    else return -1;
}

void delete_matrix(Bitmatrix *bm) {
    if (bm->matrix) {
        for (int i = 0; i < bm->size; i++)
            free(bm->matrix[i]);
        free(bm->matrix);
    }
    bm->matrix = NULL;
    bm->size = 0;
}
