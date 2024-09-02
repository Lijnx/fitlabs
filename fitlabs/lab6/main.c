#include "avltree.h"

Node *fill_nodes(size_t len) {
    Node *nodes = (Node*)malloc(len * sizeof(Node));
    if (nodes) {
        for (size_t i = 0; i < len; i++) {
            int n;
            if (scanf("%d", &n) == 1) {
                nodes[i] = create_node(n);
            }
            else {
                free(nodes);
                return NULL;
            }
        }
        return nodes;
    }
    else return NULL;
}

int main(void) {
    size_t len;
    if (scanf("%zu", &len) == 1) {
        Node *nodes = fill_nodes(len);
        if (nodes) {
            Node *root = NULL;
            for (size_t i = 0; i < len; i++)
                add(&root, &nodes[i]);
            printf("%d", get_height(root));
            free(nodes);
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}
