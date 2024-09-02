#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

typedef struct Item {
    Node *value;
    unsigned long long priority;
    struct Item *next;
    struct Item *prev;
} Item;

typedef struct Queue {
    size_t length;
    Item *first;
    Item *last;
} Queue;

void init_queue(Queue *queue);
int is_empty(Queue queue);
void enqueue(Queue *queue, Node *node, unsigned long long priority);
Node *get_node(Queue queue);
unsigned long long get_priority(Queue queue);
void dequeue(Queue *queue);
void delete_queue(Queue *queue);
