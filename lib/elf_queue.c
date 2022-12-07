#include "elf_queue.h"
#include <stdlib.h>

typedef struct elf_queue_s * elf_queue_t;


elf_status_t elf_queue_new(elf_queue_t *ref_queue);
elf_status_t elf_queue_delete(elf_queue_t *ref_queue);
elf_status_t elf_queue_enqueue(elf_queue_t queue, elf_event_t event);
elf_status_t elf_queue_dequeue(elf_queue_t queue, elf_event_t * ref_event);

struct elf_queue_s {
    size_t cap;
    size_t size;
    elf_event_t events[ELF_QUEUE_CAP]; // simple implementation
    // TODO add multi-threading support
};

/*
 * Creates a new multi-threaded unbounded queue. Capacity is fixed for simplicity.
 */
elf_status_t elf_queue_new(elf_queue_t *ref_queue) {
    // TODO
    return ELF_OK;
}

/*
 * Delete queue.
 */
elf_status_t elf_queue_delete(elf_queue_t *ref_queue) {
    // TODO

    *ref_queue = NULL; // invalidate caller pointer
    return ELF_OK;
}


/*
 * Enqueues a void pointer. If the queue has reached maximal capacity, ELF_FULL is returned;
 * else, ELF_OK is returns. DOES NOT BLOCK.
 */
elf_status_t elf_queue_enqueue(elf_queue_t queue, elf_event_t event) {
    // TODO
    return ELF_OK;
}

/*
 * Dequeues a void pointer. If the queue is empty, the function BLOCKS. Return ELF_OK.
 */
elf_status_t elf_queue_dequeue(elf_queue_t queue, elf_event_t * ref_event) {
    // TODO
    return ELF_OK;
}
