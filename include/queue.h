#ifndef __LINKED_LIST_QUEUE_H__
#define __LINKED_LIST_QUEUE_QUEUE_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct node {
    int          value_;
    struct node* next_;
} node_t;

node_t* node_create(int value, node_t* next);

typedef struct queue {
    size_t  size_;
    node_t* head_;
    node_t* tail_; 
} queue_t;

queue_t* queue_create();

bool queue_empty(const queue_t* queue);

size_t queue_size(const queue_t* queue);

int queue_front(const queue_t* queue);

int queue_back(const queue_t* queue);

void queue_push(queue_t* queue, const int item);

void queue_pop(queue_t* queue);

void queue_dump(queue_t* queue);

void queue_destroy(queue_t* queue);

#endif