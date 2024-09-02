#include "avltree.h"

Node create_node(int value) {
    Node node;
    node.value = value;
    node.height = 1;
    node.left = NULL;
    node.right = NULL;
    return node;
}

int get_height(Node* node) {
    return (node) ? node->height : 0;
}

void fix_height(Node* node) {
    int hr = get_height(node->right);
    int hl = get_height(node->left);
    node->height = ((hl > hr) ? hl : hr) + 1;
}

int height_diff(Node* node) {
    return (get_height(node->right) - get_height(node->left));
}

Node* rotate_right(Node* a) {
    Node* b = a->left;
    a->left = b->right;
    b->right = a;
    fix_height(a);
    fix_height(b);
    return b;
}

Node* rotate_left(Node* a) {
    Node* b = a->right;
    a->right = b->left;
    b->left = a;
    fix_height(a);
    fix_height(b);
    return b;
}

Node* balance(Node* node) {
    fix_height(node);
    if (height_diff(node) == 2) {
        if (height_diff(node->right) < 0)
            node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    if (height_diff(node) == -2) {
        if (height_diff(node->left) > 0)
            node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    return node;
}

void add(Node **node, Node* new) {
    if (*node) {
        if (new->value >= (*node)->value) {
            if ((*node)->right != NULL)
                add(&(*node)->right, new);
            else (*node)->right = new;
            *node = balance(*node);
        }
        else {
            if ((*node)->left != NULL)
                add(&(*node)->left, new);
            else (*node)->left = new;
            *node = balance(*node);
        }
    }
    else *node = new;
}
