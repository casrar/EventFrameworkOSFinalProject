#include "elf_queue.h"
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

typedef struct elf_queue_s * elf_queue_t;


elf_status_t elf_queue_new(elf_queue_t *ref_queue);
elf_status_t elf_queue_delete(elf_queue_t *ref_queue);
elf_status_t elf_queue_enqueue(elf_queue_t queue, elf_event_t event);
elf_status_t elf_queue_dequeue(elf_queue_t queue, elf_event_t * ref_event);


struct elf_queue_s {
    size_t cap;
    size_t size;
    elf_event_t events[ELF_QUEUE_CAP]; // simple implementation
    size_t queue_head;
    size_t queue_tail;
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_empty;  
};

/*
 * Creates a new multi-threaded unbounded queue. Capacity is fixed for simplicity.
 */
elf_status_t elf_queue_new(elf_queue_t *ref_queue) {
    (*ref_queue) = malloc(sizeof(elf_queue_t));
    assert(*ref_queue != NULL); // If malloc fail, crash
    (**ref_queue).cap = ELF_QUEUE_CAP;
    (**ref_queue).size = 0;
    (**ref_queue).queue_head = 0;
    (**ref_queue).queue_tail = 0; 
    pthread_mutex_init(&(**ref_queue).queue_mutex, NULL);
    pthread_cond_init(&(**ref_queue).queue_empty, NULL);
    return ELF_OK;
}

/*
 * Delete queue.
 */
elf_status_t elf_queue_delete(elf_queue_t *ref_queue) {
    free(*ref_queue);
    *ref_queue = NULL; // invalidate caller pointer
    return ELF_OK;
}


/*
 * Enqueues a void pointer. If the queue has reached maximal capacity, ELF_FULL is returned;
 * else, ELF_OK is returns. DOES NOT BLOCK.
 */
elf_status_t elf_queue_enqueue(elf_queue_t queue, elf_event_t event) {
    if ((*queue).size >= (*queue).cap) {
        return ELF_FULL;
    }
    (*queue).events[(*queue).size] = event; // Add event to events []
    (*queue).size++; // Increase size
    (*queue).queue_tail++; // Increase tail
    pthread_cond_signal(&(*queue).queue_empty); // Signal events ! empty
    return ELF_OK;
}

/*
 * Dequeues a void pointer. If the queue is empty, the function BLOCKS. Return ELF_OK.
 */
elf_status_t elf_queue_dequeue(elf_queue_t queue, elf_event_t * ref_event) {
    pthread_mutex_lock(&(*queue).queue_mutex); 
    while ((*queue).size <= 0) {
        pthread_cond_wait(&(*queue).queue_empty, &(*queue).queue_mutex); // Waits till event in queue
    }
    *ref_event = (*queue).events[(*queue).queue_tail]; // Sef ref_event to tail
    (*queue).size--;
    (*queue).queue_tail--;
    pthread_mutex_unlock(&(*queue).queue_mutex); // Unlock mutex when consumed

    return ELF_OK;
}