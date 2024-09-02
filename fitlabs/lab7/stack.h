#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef struct Item {
    int value;
    struct Item *next;
} Item;

typedef struct Stack {
    Item *top;
    size_t len;
} Stack;

void init_stack(Stack *stack);
int get_top(Stack stack);
int is_empty(Stack stack);
void push(Stack *stack, int value);
int pop(Stack *stack);
void print_stack(Stack stack);
void delete_stack(Stack *stack);
