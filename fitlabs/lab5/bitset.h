#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitio.h"

enum {MAX_BYTES = 32};
enum {MAX_SIZE = 256};

typedef struct Bitset {
    size_t size;
    unsigned char bitset[MAX_BYTES];
} Bitset;

void init_bitset(Bitset *bs);
int bitset_is_full(Bitset bs);
void append_bit(Bitset *bs, unsigned char bit);
int get_bit(Bitset bs, size_t pos);
int rewrite_at(Bitset *bs, size_t pos, unsigned char bit);
void copy_bitset(Bitset *dst, Bitset src);
void print_bitset(Bitset bs, Writer *writer);
