#include "fibheap.h"

int fb_log2(int x) {
    int result = 0;
    while (x) {
        x >>= 1;
        result++;
    }
    return result;
}

Fibheap fb_init() {
    Fibheap fb;
    fb.min = NULL;
    fb.size = 0;
    return fb;
}

int fb_is_empty(Fibheap *fb) {
    return (fb->min == NULL) ? 1 : 0;
}

Node *fb_merge_lists(Node *first, Node *second) {
    if (first == NULL && second == NULL)
        return NULL;
    else if (first != NULL && second == NULL)
        return first;
    else if (first == NULL)
        return second;
    else {
        Node *tmp = first->right;
        first->right = second->right;
        (first->right)->left = first;
        second->right = tmp;
        (second->right)->left = second;
        return (first->key < second->key) ? first : second;
    }
}

Node* fb_insert(Fibheap *fb, int value, long long key) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fputs("memory error\n", stderr);
        return NULL;
    }

    new_node->key = key;
    new_node->value = value;
    new_node->parent = NULL;
    new_node->child = NULL;
    new_node->left = new_node;
    new_node->right = new_node;
    new_node->degree = 0;
    new_node->mark = 0;
    if (fb->min == NULL)
        fb->min = new_node;
    else
        fb->min = fb_merge_lists(fb->min, new_node);
    fb->size++;
    return new_node;
}
/*
void fb_union(Fibheap *fb, Fibheap *add) {
    if (add->min == NULL)
        return;
    else if (fb->min == NULL) {
        fb->min = add->min;
        fb->size = add->size;
    }
    else {
        fb->min = fb_merge_lists(fb->min, add->min);
        fb->size += add->size;

        add->min = NULL;
        add->size = 0;
    }
}
*/
void fb_consolidate(Fibheap *fb) {
    if (fb->min == NULL)
        return;
    if (fb->min->right == fb->min)
        return;

    Node **root_list = (Node**)calloc(fb_log2(fb->size) + 1, sizeof(Node*));
    if (root_list == NULL) {
        fputs("memory error\n", stderr);
        return;
    }

    Node *curr = fb->min;
    do {
        if (root_list[curr->degree] == NULL) {
            root_list[curr->degree] = curr;
            curr = curr->right;
        } else {
            Node *other = root_list[curr->degree];
            root_list[curr->degree] = NULL;

            Node *min = (curr->key < other->key) ? curr : other;
            Node *max = (curr->key < other->key) ? other : curr;

            (max->left)->right = max->right;
            (max->right)->left = max->left;

            max->left = max;
            max->right = max;
            min->child = fb_merge_lists(min->child, max);
            max->parent = min;

            min->degree++;
            curr = min;
        }
    } while (curr != fb->min);
    free(root_list);
}

void fb_find_min(Fibheap *fb) {
    if (fb->min == NULL)
        return;
    if (fb->min->right == fb->min)
        return;
    else {
        Node *ptr = fb->min;
        Node *head = fb->min;
        do {
            if (ptr->key < fb->min->key)
                fb->min = ptr;
            ptr = ptr->right;
        } while (ptr != head);
    }
}

Node *fb_extract_min(Fibheap *fb) {
    if (fb_is_empty(fb))
        return NULL;

    Node* min = fb->min;
    //Delete min from root list
    if (fb->min->right == fb->min)
        fb->min = NULL;
    else {
        (fb->min->left)->right = fb->min->right;
        (fb->min->right)->left = fb->min->left;
        fb->min = fb->min->right;
    }
    fb->size--;

    //Delete parent node from min child list
    if (min->child != NULL) {
        Node* curr = min->child;
        do {
            curr->parent = NULL;
            curr = curr->right;
        } while (curr != min->child);
    }

    //Merge min child list into root list
    fb->min = fb_merge_lists(fb->min, min->child);
    fb_find_min(fb);
    fb_consolidate(fb);

    return min;
}

void fb_cut(Fibheap *fb, Node* node) {
    (node->left)->right = node->right;
    (node->right)->left = node->left;
    (node->parent)->degree--;
    if ((node->parent)->child == node) {
        if (node->right == node)
            (node->parent)->child = NULL;
        else
            (node->parent)->child = node->right;
    }
    node->left = node;
    node->right = node;
    fb->min = fb_merge_lists(fb->min, node);
    node->parent = NULL;
    node->mark = 0;
}

void fb_cascade_cut(Fibheap *fb, Node* node) {
    Node* parent = node->parent;
    if (parent != NULL) {
        if (node->mark == 0)
            node->mark = 1;
        else {
            fb_cut(fb, node);
            fb_cascade_cut(fb, parent);
        }
    }
}

void fb_decrease_key(Fibheap *fb, Node* node, long long new_key) {
    if (fb_is_empty(fb))
        return;

    Node* parent = node->parent;
    node->key = new_key;

    if (parent != NULL && node->key < parent->key) {
        fb_cut(fb, node);
        fb_cascade_cut(fb, parent);
    }

    if (node->key < (fb->min)->key)
        fb->min = node;
}
