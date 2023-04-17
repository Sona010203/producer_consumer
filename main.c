#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

#include "queue.h"
#include "error_messages.h"

typedef struct sync_context {
    pthread_mutex_t mutex;
    pthread_cond_t buffer_full;
    pthread_cond_t buffer_empty;
    queue_t* queue_;   
} sync_context_t;

void* execute_producer(void* input)
{
    sync_context_t* context = (sync_context_t*)input;

    for (size_t i = 0; i < 25UL; i++) {
        sleep(1);

        const int new_item = rand() % 999;
        
        // Enter critical section
        pthread_mutex_lock(&context->mutex);

        while(context->queue_->size_ > 10) {

            pthread_cond_wait(&context->buffer_full, &context->mutex);

        }

        printf("New item %3d is produced and inserted into the storage: ", new_item);

        queue_push(context->queue_, new_item);
        queue_dump(context->queue_);

        // Leave critical section
        pthread_mutex_unlock(&context->mutex);
        pthread_cond_signal(&context->buffer_empty);
    }

    return NULL;
}

void* execute_consumer(void* input)
{
    sync_context_t* context = (sync_context_t*)input;

    for (size_t i = 0; i < 25UL; i++) {
        sleep(1);

        // Enter critical section
        pthread_mutex_lock(&context->mutex);
        while((*context->queue_).size_ == 0) {

            pthread_cond_wait(&context->buffer_empty, &context->mutex);

        }

        const int item = queue_front(context->queue_);
        printf("New item %3d is consumed and removed from the storage:  ", item);

        queue_pop(context->queue_);
        queue_dump(context->queue_);

        // Leave critical section
        pthread_mutex_unlock(&context->mutex);
        pthread_cond_signal(&context->buffer_full);
    }

    return NULL;
}

int main(int argc, char** argv)
{
    if (argc != 4) {
        printf(ERROR_FORMAT, ARGS_ERROR);
        return 1;
    }   

    // Parse command line arguments
    const size_t queue_size = atoi(argv[1]);
    const size_t producers_count = atoi(argv[2]);
    const size_t consumers_count = atoi(argv[3]);

    // Initialize synchronization context
    sync_context_t sync_context;
    sync_context.queue_ = queue_create();
    pthread_mutex_init(&sync_context.mutex, NULL);
    pthread_cond_init(&sync_context.buffer_full, NULL);
    pthread_cond_init(&sync_context.buffer_empty, NULL);
    
    // Initialize random machine
    srand(time(NULL));

    // Create consumer threads
    pthread_t consumers[consumers_count];
    for (size_t i = 0; i < consumers_count; i++) {
        pthread_create(&consumers[i], NULL, execute_consumer, &sync_context);
        // join why not?
    }

    // Create producer threads
    pthread_t producers[producers_count];
    for (size_t i = 0; i < producers_count; i++)
        pthread_create(&producers[i], NULL, execute_producer, &sync_context);

    // Join producer threads
    for (size_t i = 0; i < producers_count; i++)
        pthread_join(producers[i], NULL);

    // Join consumer threads 
    for (size_t i = 0; i < consumers_count; i++)
        pthread_join(consumers[i], NULL);
    
    // Destroy synchronization context
    pthread_mutex_destroy(&sync_context.mutex);
    pthread_cond_destroy(&sync_context.buffer_full);
    pthread_cond_destroy(&sync_context.buffer_empty);
    queue_destroy(sync_context.queue_);

    return 0;
}
