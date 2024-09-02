#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int value;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

Node create_node(int value);
int get_height(Node* node);
void add(Node **node, Node* new);
