#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    FILE* output;
    unsigned char byte;
    size_t bits_filled;
} Writer;

typedef struct {
    FILE* input;
    unsigned char byte;
    unsigned char buf;
    size_t bits_filled;
} Reader;

void init_writer(Writer *writer, FILE* output);
void write_bit(Writer *writer, unsigned char bit);
void write_byte(Writer *writer, unsigned char byte);
void write_number(Writer *writer, unsigned number, size_t bits);
void write_last(Writer *writer);

void init_reader(Reader *reader, FILE* input);
unsigned char read_bit(Reader *reader);
unsigned char read_byte(Reader *reader);
unsigned read_number(Reader *reader, size_t bits);
