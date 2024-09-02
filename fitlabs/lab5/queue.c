#include "queue.h"

void init_queue(Queue *queue) {
    queue->length = 0;
    queue->last = NULL;
    queue->first = NULL;
}

int is_empty(Queue queue) {
    return (queue.length == 0) ? 1 : 0;
}

Item* new_item(Node *node, unsigned long long priority, Item *prev, Item *next) {
    Item* new = (Item*)malloc(sizeof(Item));
    if (new) {
        new->value = node;
        new->priority = priority;
        new->prev = prev;
        new->next = next;
    }
    return new;
}

void enqueue(Queue *queue, Node *node, unsigned long long priority) {
    Item* new = new_item(node, priority, NULL, NULL);
    if (new) {
        if (!is_empty(*queue)) {
            if (new->priority <= queue->first->priority) {
                queue->first->prev = new;
                new->next = queue->first;
                queue->first = new;
                queue->length++;
            }
            else if (new->priority >= queue->last->priority) {
                queue->last->next = new;
                new->prev = queue->last;
                queue->last = new;
                queue->length++;
            }
            else {
                for (Item *l = queue->first->next, *r = queue->last->prev; ; l = l->next, r = r->prev) {
                    if (new->priority <= l->priority) {
                        Item *tmp = l->prev;
                        tmp->next = new;
                        new->prev = tmp;
                        l->prev = new;
                        new->next = l;
                        queue->length++;
                        break;
                    }
                    if (new->priority >= r->priority) {
                        Item *tmp = r->next;
                        tmp->prev = new;
                        new->next = tmp;
                        r->next = new;
                        new->prev = r;
                        queue->length++;
                        break;
                    }
                }
            }
        }
        else {
            queue->first = new;
            queue->last = new;
            queue->length++;
        }
    }
    else {
        fputs("Stack Overflow", stderr);
        delete_tree(node);
    }
}

Node *get_node(Queue queue) {
    if (!is_empty(queue)) {
        return queue.first->value;
    }
    else return NULL;
}

unsigned long long get_priority(Queue queue) {
    if (!is_empty(queue))
        return queue.first->priority;
    else return 0;
}

void dequeue(Queue *queue) {
    if (!is_empty(*queue)) {
        if (queue->first == queue->last) {
            free(queue->first);
            queue->first = queue->last = NULL;
        }
        else {
            Item *tmp = queue->first->next;
            tmp->prev = NULL;
            free(queue->first);
            queue->first = tmp;
        }
        queue->length--;
    }
    else fputs("Queue Underflow", stderr);
}

void delete_queue(Queue *queue) {
    while (!is_empty(*queue)) {
        delete_tree(get_node(*queue));
        dequeue(queue);
    }
}
