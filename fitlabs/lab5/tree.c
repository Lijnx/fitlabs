#include "tree.h"

int is_leaf(Node *node) {
    if (node)
        return (!node->left && !node->right) ? 1 : 0;
    else return -1;
}

Node* new_node(unsigned char value, Node *left, Node *right) {
    Node* new = (Node*)malloc(sizeof(Node));
    if (new) {
        new->value = value;
        new->left = left;
        new->right = right;
    }
    return new;
}

void delete_tree(Node *node) {
    if (node) {
        delete_tree(node->left);
        delete_tree(node->right);
        free(node);
    }
}
