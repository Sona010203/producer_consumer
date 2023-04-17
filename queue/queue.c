#include <stdlib.h>
#include <stdio.h>

#include "queue.h"

node_t* node_create(int value, node_t* next)
{
    node_t* node = malloc(sizeof(node_t));
    if (node == NULL)
        return node;
    
    node->value_ = value;
    node->next_  = next;

    return node;
}

queue_t* queue_create()
{
    queue_t* inst = malloc(sizeof(queue_t));
    if (inst == NULL)
        return NULL;
    
    inst->size_ = 0;
    inst->head_ = NULL;
    inst->tail_ = NULL;

    return inst;
}

bool queue_empty(const queue_t* queue)
{
    return queue->size_ == 0;
}

size_t queue_size(const queue_t* queue)
{
    return queue->size_;
}

int queue_front(const queue_t* queue)
{
    return queue->head_->value_;
}

int queue_back(const queue_t* queue)
{
    return queue->tail_->value_;
}

void queue_push(queue_t* queue, const int item)
{
    queue->size_++;
    if (queue->size_ == 1) {
        queue->head_ = node_create(item, NULL);
        queue->tail_ = queue->head_;
        return;
    }

    queue->tail_->next_ = node_create(item, NULL);
    queue->tail_        = queue->tail_->next_; 
}

void queue_pop(queue_t* queue)
{
    node_t* head = queue->head_;
    queue->head_ = queue->head_->next_;
    free(head);
    if (--queue->size_ == 0)
        queue->tail_ = NULL;
}

void queue_dump(queue_t* queue)
{
    if (queue->size_ == 0) {
        printf("Queue is empty!\n");
        return;
    }
    node_t* it = queue->head_;
    while (it != NULL) {
        printf("%d ", it->value_);
        it = it->next_;       
    }
    printf("\n");
}

void queue_destroy(queue_t* queue)
{
    node_t* it = queue->head_;
    while (it != NULL) {
        node_t* delete_node = it;
        it = it->next_;
        free(delete_node);
    }
    free(queue);
    queue = NULL;
}