#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "tree.h"
#include "bitset.h"
#include "bitio.h"

enum {BUFFER_SIZE = 4096};
enum {ALPHABET_SIZE = 256};

//Gets last byte offset (last byte bits filled)
size_t get_lbo(const unsigned long long *freq_table, Bitset *code_table) {
    size_t result = 0;
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        result += ((freq_table[i] % 8) * (code_table[i].size % 8)) % 8;
    }
    return (result % 8);
}

void encode_node(Writer *writer, Node *node) {
    if (is_leaf(node)) {
        write_bit(writer, 1);
        write_byte(writer, node->value);
    }
    else {
        write_bit(writer, 0);
        encode_node(writer, node->left);
        encode_node(writer, node->right);
    }
}

Node* read_node(Reader *reader) {
    if (read_bit(reader) == 1)
        return new_node(read_byte(reader), NULL, NULL);
    else {
        Node* left = read_node(reader);
        Node* right = read_node(reader);
        return new_node(0, left, right);
    }
}

void create_freq_table(FILE* input, unsigned long long *freq_table) {
    unsigned char buffer[BUFFER_SIZE] = "";
    size_t read = fread(buffer, sizeof(char), BUFFER_SIZE, input);
    while(read != 0) {
        for (size_t i = 0; i < read; i++)
            freq_table[buffer[i]]++;
        read = fread(buffer, sizeof(char), BUFFER_SIZE, input);
    }
}

Node* generate_tree(unsigned long long *freq_table) {
    //Initialize nodes priority queue
    Queue queue;
    init_queue(&queue);
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        if (freq_table[i] != 0) {
            Node* node = (Node*)malloc(sizeof(Node));
            node->value = i;
            node->left = NULL;
            node->right = NULL;
            enqueue(&queue, node, freq_table[i]);
        }
    }

    if (!is_empty(queue)) {
        //Build Huffman tree
        while (queue.length > 1) {
            unsigned long long priority = 0;

            priority += get_priority(queue);
            Node* left_node = get_node(queue);
            dequeue(&queue);

            priority += get_priority(queue);
            Node* right_node = get_node(queue);
            dequeue(&queue);

            Node* node = new_node(0, left_node, right_node);
            if (node)
                enqueue(&queue, node, priority);
            else {
                fputs("Stack Overflow", stderr);
                delete_queue(&queue);
                free(left_node);
                free(right_node);
                return NULL;
            }
        }
        Node* root = get_node(queue);
        dequeue(&queue);
        return root;
    }
    else return NULL;
}

void generate_bitsets(Node* node, Bitset bs, Bitset* code_table) {
    if (node) {
        if (is_leaf(node)) {
            if (bs.size == 0)
                append_bit(&bs, 1);
            copy_bitset(&code_table[node->value], bs);
        }
        else {
            append_bit(&bs, 1);
            generate_bitsets(node->right, bs, code_table);
            rewrite_at(&bs, bs.size - 1, 0);
            generate_bitsets(node->left, bs, code_table);
        }
    }
}

void compress(FILE *input, Writer *writer, Bitset* code_table) {
    unsigned char buffer[BUFFER_SIZE] = "";

    size_t read = fread(buffer, sizeof(char), BUFFER_SIZE, input);
    while (read != 0) {
        for (size_t i = 0; i < read; i++)
            print_bitset(code_table[buffer[i]], writer);

        read = fread(buffer, sizeof(char), BUFFER_SIZE, input);
    }
}

void decompress(Reader *reader, FILE* output, Node *h_tree, size_t lbo) {
    Node *node = h_tree;
    //Second condition is needed to handle last byte
    while (!feof(reader->input) || reader->bits_filled != (8 - lbo)) {
        unsigned char bit = read_bit(reader);
        if (!is_leaf(h_tree)) {
            if (bit == 1)
                node = node->right;
            else
                node = node->left;
        }
        if (is_leaf(node)) {
            fputc(node->value, output);
            node = h_tree;
        }
    }
}

void archiver(FILE* input, FILE* output, char mode) {
    if (mode == 'c') {
        long pos = ftell(input);
        unsigned long long freq_table[ALPHABET_SIZE] = { 0 };
        create_freq_table(input, freq_table);
        fseek(input, pos, SEEK_SET);

        Node* root = generate_tree(freq_table);
        if (root) {
            Bitset code_table[ALPHABET_SIZE];
            for (size_t i = 0; i < ALPHABET_SIZE; i++)
                init_bitset(&code_table[i]);

            Bitset bs;
            init_bitset(&bs);
            generate_bitsets(root, bs, code_table);

            Writer writer;
            init_writer(&writer, output);

            encode_node(&writer, root);
            size_t lbo = (writer.bits_filled + 3 + get_lbo(freq_table, code_table)) % 8;
            write_number(&writer, lbo, 3);
            compress(input, &writer, code_table);
            write_last(&writer);

            delete_tree(root);
        }
    }

    if (mode == 'd') {
        Reader reader;
        init_reader(&reader, input);
        if (!feof(reader.input)) {
            Node* root = read_node(&reader);
            size_t lbo = read_number(&reader, 3);
            lbo = (lbo == 0) ? 8 : lbo;

            decompress(&reader, output, root, lbo);
            delete_tree(root);
        }
    }
}

void console_handler(int argc, char** argv) {
    if (argc == 4) {
        if (strcmp(argv[1], "c") == 0 || strcmp(argv[1], "d") == 0) {
            char mode = argv[1][0];
            FILE* input = fopen(argv[2], "rb");
            FILE* output = fopen(argv[3], "wb");
            if (input && output) {
                archiver(input, output, mode);
            }
            fclose(input);
            fclose(output);
        }
    }
}

void default_handler(char* infile_name, char* outfile_name) {
    FILE* input = fopen(infile_name, "rb");
    FILE* output = fopen(outfile_name, "wb");
    if (input && output) {
        char mode = (char)fgetc(input);
        archiver(input, output, mode);
    }
    fclose(input);
    fclose(output);
}

int main(int argc, char** argv) {

    if (argc == 1)
        default_handler("in.txt", "out.txt");
    else
        console_handler(argc, argv);

    return EXIT_SUCCESS;
}
