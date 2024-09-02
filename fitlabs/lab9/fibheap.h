#ifndef __FIBHEAP_H__
#define __FIBHEAP_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    long long key;
    int value;
    struct Node *parent;
    struct Node *child;
    struct Node *left;
    struct Node *right;
    int degree;             //size of child-list
    char mark;              //is needed for cascade-cut
} Node;

typedef struct Fibheap {
    int size;
    Node *min;
} Fibheap;

Fibheap fb_init();
int fb_is_empty(Fibheap *fb);
Node* fb_insert(Fibheap *fb, int value, long long key);
//void fb_union(Fibheap *fb, Fibheap *add);
Node *fb_extract_min(Fibheap *fb);
void fb_decrease_key(Fibheap *fb, Node* node, long long new_key);

#endif //__FIBHEAP_H__
