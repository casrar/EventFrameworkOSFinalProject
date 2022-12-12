#include "elf_internal.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

// global state
size_t elf_num_loops;
elf_loop_t elf_loops[ELF_CAP_LOOPS];
bool elf_loops_valid[ELF_CAP_LOOPS];

// locks global loops array; maybe replaced with rw-lock
pthread_mutex_t elf_loop_lock = PTHREAD_MUTEX_INITIALIZER;

extern inline elf_event_t elf_event_token();
extern inline elf_event_t elf_event_loop_id(uint32_t loop_id);
extern inline elf_event_t elf_event_ptr(void * ptr);
extern inline elf_event_t elf_event_int32(int32_t loop_id);

// thread routine used for all event loops; this function contains
//   the loop that processes all events for an event loop
//
void * elf_loop_routine(void * id) {
    assert(id != NULL);
    elf_loop_t loop = (elf_loop_t) id;
    elf_status_t status;
    elf_event_t event;

    loop->state = ELF_LOOP_STATE_ACTIVE;

    while (1) {
        // dequeue message or block
        // TODO
        elf_queue_dequeue(loop->queue, &event);

        // invoke registered event handler callback
        status = loop->handler(loop->id, event);
        if (status != ELF_OK)
            break; // one way to leave event processing loop
    }

    loop->state = ELF_LOOP_STATE_DONE;
    return NULL;
}


elf_status_t elf_main(elf_handler_t handler) {
    printf("start");
    assert(elf_num_loops == 0);
    assert(handler != NULL);

    elf_status_t status = elf_loop_new(elf_loops + 0, 0, handler);
    if (status != ELF_OK)
        return status;

    // update global number of loops; no mutex needed because
    //   we assume single main thread until main loop starts
    elf_loops_valid[0] = true;
    elf_num_loops += 1;

    status = elf_loop_start(elf_loops[0]);
    if (status != ELF_OK)
        return status;

    pthread_join(elf_loops[0]->thread, NULL);
    return ELF_OK;
}


// creates new event loop if possible
elf_status_t elf_init(uint32_t *ref_loop_id, elf_handler_t handler) {
    
    pthread_mutex_lock(&elf_loop_lock);

    for(int i = 0; i < sizeof(elf_loops); i++){
        if(elf_loops[i]->id = *ref_loop_id){
            pthread_mutex_unlock(&elf_loop_lock);
            return ELF_ERROR;
        }
        elf_status_t out = elf_loop_new(elf_loops + *ref_loop_id, *ref_loop_id,handler);
        if(out != ELF_OK){
            return out;
        }
        elf_status_t out2 = elf_loop_start(elf_loops[*ref_loop_id]);
        if(out != ELF_OK){
            return out2;
        }
        pthread_mutex_unlock(&elf_loop_lock);
    }
    return ELF_OK;
}


// ends event loop if possible
elf_status_t elf_fini(uint32_t loop_id) {
    pthread_mutex_lock(&elf_loop_lock);
    free(elf_loops[loop_id]);
    pthread_mutex_unlock(&elf_loop_lock);
    return ELF_OK;
}


// sends an event to an event loop if possible
elf_status_t elf_send(uint32_t loop_id, elf_event_t event) {
    elf_queue_enqueue(elf_loops[loop_id]->queue, event);
    return ELF_OK;
}


elf_status_t elf_loop_new(elf_loop_t *ref_loop, uint32_t id, elf_handler_t handler) {
    printf("new loop created");
    assert(ref_loop != NULL);
    assert(*ref_loop == NULL);
    assert(handler != NULL);

    // allocate memory on heap
    *ref_loop = calloc(1, sizeof **ref_loop);
    if (*ref_loop == NULL)
        return ELF_ERROR;

    // create new queue
    elf_queue_t queue;
    if (elf_queue_new(&queue) != ELF_OK) {
        free(*ref_loop);
        return ELF_ERROR;
    }

    // set struct fields
    (**ref_loop).id = id;
    (**ref_loop).state = ELF_LOOP_STATE_INIT;
    (**ref_loop).handler = handler;
    (**ref_loop).queue = queue;

    return ELF_OK;
}


elf_status_t elf_loop_start(elf_loop_t loop) {
    assert(loop != NULL);
    assert(loop->state == ELF_LOOP_STATE_INIT);

    // create event loop thread
    int result = pthread_create(&loop->thread, NULL, elf_loop_routine, (void *) loop);
    if (result != 0)
        return ELF_ERROR;

    return ELF_OK;
}
