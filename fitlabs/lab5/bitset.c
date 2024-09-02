#include "bitset.h"

void init_bitset(Bitset *bs) {
    bs->size = 0;
    memset(bs->bitset, 0, MAX_BYTES);
}

int bitset_is_full(Bitset bs) {
    return (bs.size == MAX_SIZE) ? 1 : 0;
}

void append_bit(Bitset *bs, unsigned char bit) {
    if (!bitset_is_full(*bs)) {
        size_t first_byte = (MAX_BYTES - 1) - (bs->size >> 3);
        for (size_t i = first_byte; i < MAX_BYTES - 1; i++) {
            bs->bitset[i] <<= 1;
            bs->bitset[i] += ((bs->bitset[i + 1] >> 7) & 1);
        }
        bs->bitset[MAX_BYTES - 1] <<= 1;
        bs->bitset[MAX_BYTES - 1] += (bit & 1);
        bs->size++;
    }
    else fputs("Bitset Overflow", stderr);
}

int get_bit(Bitset bs, size_t pos) {
    if (pos < bs.size) {
        size_t byte = (MAX_SIZE - bs.size + pos) >> 3;
        size_t offset = (MAX_SIZE - bs.size + pos) - (byte << 3);
        return (bs.bitset[byte] >> (7 - offset)) & 1;
    }
    else return -1;
}

int rewrite_at(Bitset *bs, size_t pos, unsigned char bit) {
    if (pos < bs->size) {
        size_t byte = (MAX_SIZE - bs->size + pos) >> 3;
        size_t offset = (MAX_SIZE - bs->size + pos) - (byte << 3);
        if (bit)
            bs->bitset[byte] |= (1 << (7 - offset));
        else
            bs->bitset[byte] &= ~(1 << (7 - offset));
        return (bs->bitset[byte] >> (7 - offset)) & 1;
    }
    else return -1;
}

void copy_bitset(Bitset *dst, Bitset src) {
    for (size_t i = 0; i < src.size; i++)
        append_bit(dst, get_bit(src, i));
}

void print_bitset(Bitset bs, Writer *writer) {
    for (size_t i = 0; i < bs.size; i++)
        write_bit(writer, get_bit(bs, i));
}
