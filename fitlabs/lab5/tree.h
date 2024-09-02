#pragma once
#include <stdlib.h>

typedef struct Node {
    unsigned char value;
    struct Node* left;
    struct Node* right;
} Node;

int is_leaf(Node *node);
Node* new_node(unsigned char value, Node *left, Node *right);
void delete_tree(Node *node);
