#include "stack.h"

void init_stack(Stack *stack) {
    stack->top = NULL;
    stack->len = 0;
}

int get_top(Stack stack) {
    return stack.top->value;
}

int is_empty(Stack stack) {
    return (stack.len == 0) ? 1 : 0;
}

void push(Stack *stack, int value) {
    Item *new = (Item*)calloc(1, sizeof(Item));
    if (new != NULL) {
        new->value = value;
        new->next = stack->top;
        stack->top = new;
        stack->len++;
    }
    else fputs("Stack::Stack Overflow\n", stderr);
}

int pop(Stack *stack) {
    if (!is_empty(*stack)) {
        int value = get_top(*stack);
        Item *next = stack->top->next;
        free(stack->top);
        stack->top = next;
        stack->len--;
        return value;
    }
    else fputs("Stack::Stack Underflow\n", stderr);
    return 0;
}

void print_stack(Stack stack) {
    for (Item* i = stack.top; i != NULL; i = i->next)
        printf("%d ", i->value);
}

void delete_stack(Stack *stack) {
    while (!is_empty(*stack))
        pop(stack);
    init_stack(stack);
}
