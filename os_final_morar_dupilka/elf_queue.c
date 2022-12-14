#include "elf_queue.h"
#include <stdlib.h>
#include <pthread.h>

typedef struct elf_queue_s * elf_queue_t;


elf_status_t elf_queue_new(elf_queue_t *ref_queue);
elf_status_t elf_queue_delete(elf_queue_t *ref_queue);
elf_status_t elf_queue_enqueue(elf_queue_t queue, elf_event_t event);
elf_status_t elf_queue_dequeue(elf_queue_t queue, elf_event_t * ref_event);

struct elf_queue_s {
    size_t cap;          
    size_t size;                       
    int head;                            
    int tail;                            
    elf_event_t events[ELF_QUEUE_CAP];  
    pthread_mutex_t queue_mutex;               
    pthread_cond_t queue_empty;          
};

/*
 * Creates a new multi-threaded unbounded queue. Capacity is fixed for simplicity.
 */
elf_status_t elf_queue_new(elf_queue_t *ref_queue) {

    *ref_queue = calloc(1, sizeof(struct elf_queue_s));

    (*ref_queue)->cap = ELF_QUEUE_CAP;
    (*ref_queue)->size = 0;
    (*ref_queue)->head = 0;
    (*ref_queue)->tail = 0;

    pthread_mutex_init(&((*ref_queue)->queue_mutex), NULL);
    pthread_cond_init(&((*ref_queue)->queue_empty), NULL);

    return ELF_OK;
}

/*
 * Delete queue.
 */
elf_status_t elf_queue_delete(elf_queue_t *ref_queue) {
    free(ref_queue);
    *ref_queue = NULL; // invalidate caller pointer
    return ELF_OK;
}


/*
 * Enqueues a void pointer. If the queue has reached maximal capacity, ELF_FULL is returned;
 * else, ELF_OK is returns. DOES NOT BLOCK.
 */
elf_status_t elf_queue_enqueue(elf_queue_t queue, elf_event_t event) {

    pthread_mutex_lock(&(queue->queue_mutex));

    if (queue->size == queue->cap) {
        pthread_mutex_unlock(&(queue->queue_mutex));
        return ELF_FULL;
    }

    queue->events[queue->tail] = event;
    queue->tail++;

    if (queue->tail == queue->cap){
        queue->tail = 0;
    }
    queue->size++;

    pthread_cond_signal(&queue->queue_empty);
    pthread_mutex_unlock(&(queue->queue_mutex));

    return ELF_OK;
}

/*
 * Dequeues a void pointer. If the queue is empty, the function BLOCKS. Return ELF_OK.
 */
elf_status_t elf_queue_dequeue(elf_queue_t queue, elf_event_t * ref_event) {

    pthread_mutex_lock(&(queue->queue_mutex));

    if (queue->size == 0){
        pthread_cond_wait(&(queue->queue_empty), &queue->queue_mutex);
    }

    *ref_event = queue->events[queue->head];
    queue->head++;

    if (queue->head > queue->cap){
        queue->head = 0;
    }
    queue->size--;

    pthread_mutex_unlock(&(queue->queue_mutex));

    return ELF_OK;
}