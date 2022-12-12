/*

  Internal Interface to Event Loop Framework (ELF)

*/

#pragma once

#include "elf_public.h"
#include "elf_queue.h"
#include <pthread.h>
#include <stdint.h>

enum elf_loop_state_e {
    ELF_LOOP_STATE_INIT,
    ELF_LOOP_STATE_ACTIVE,
    ELF_LOOP_STATE_DONE,
};
typedef enum elf_loop_state_e elf_loop_state_t;

struct elf_loop_s {
    uint32_t id;
    elf_loop_state_t state;
    pthread_t thread;
    elf_handler_t handler;
    elf_queue_t queue;
};
typedef struct elf_loop_s * elf_loop_t;


elf_status_t elf_loop_new(elf_loop_t *ref_loop, uint32_t id, elf_handler_t handler);
elf_status_t elf_loop_start(elf_loop_t loop);


