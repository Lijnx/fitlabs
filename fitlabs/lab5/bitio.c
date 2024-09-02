#include "bitio.h"

void init_writer(Writer *writer, FILE* output) {
    writer->output = output;
    writer->byte = 0;
    writer->bits_filled = 0;
}

void write_bit(Writer *writer, unsigned char bit) {
    writer->byte <<= 1;
    writer->byte += (bit & 1);
    writer->bits_filled++;
    if (writer->bits_filled == 8) {
        fputc(writer->byte, writer->output);
        writer->byte = 0;
        writer->bits_filled = 0;
    }
}

void write_byte(Writer *writer, unsigned char byte) {
    if (writer->bits_filled == 0) {
        fputc(byte, writer->output);
    }
    else {
        size_t shift = 8 - writer->bits_filled;
        writer->byte <<= shift;
        writer->byte += (byte >> writer->bits_filled);
        fputc(writer->byte, writer->output);
        writer->byte = (byte << shift) >> shift;
    }
}

void write_number(Writer *writer, unsigned number, size_t bits) {
    for (int j = (int)(bits - 1); j >= 0; j--) {
        write_bit(writer, (number >> j) & 1);
    }
}

void write_last(Writer *writer) {
    if (writer->bits_filled != 0) {
        writer->byte <<= (8 - writer->bits_filled);
        fputc(writer->byte, writer->output);
        writer->byte = writer->bits_filled;
        writer->byte = 0;
        writer->bits_filled = 0;
    }
}

void init_reader(Reader *reader, FILE* input) {
    reader->input = input;
    reader->byte = 0;
    reader->buf = fgetc(reader->input);
    reader->bits_filled = 0;
}

unsigned char read_bit(Reader *reader) {
    if (reader->bits_filled == 0) {
        reader->byte = reader->buf;
        reader->buf = fgetc(reader->input);
        reader->bits_filled = 8;
    }
    unsigned char bit = (reader->byte >> 7) & 1;
    reader->byte <<= 1;
    reader->bits_filled--;
    return bit;
}

unsigned char read_byte(Reader *reader) {
    if (reader->bits_filled == 0) {
        unsigned char tmp = reader->buf;
        reader->buf = fgetc(reader->input);
        return tmp;
    }
    else {
        reader->byte += (reader->buf >> reader->bits_filled);
        unsigned char tmp = reader->byte;
        reader->byte = reader->buf << (8 - reader->bits_filled);
        reader->buf = fgetc(reader->input);
        return tmp;
    }
}

unsigned read_number(Reader *reader, size_t bits) {
    unsigned result = 0;
    for (size_t i = 0; i < bits; i++) {
        result <<= 1;
        result += read_bit(reader);
    }
    return result;
}
