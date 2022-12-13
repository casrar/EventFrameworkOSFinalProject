/*

  Interface to Multi-Threaded Unbounded Event Queue

*/

#pragma once

#include "elf_public.h"
#include <stdint.h>

#define ELF_QUEUE_CAP 64

typedef struct elf_queue_s * elf_queue_t;

elf_status_t elf_queue_new(elf_queue_t *ref_queue);
elf_status_t elf_queue_delete(elf_queue_t *ref_queue);
elf_status_t elf_queue_enqueue(elf_queue_t queue, elf_event_t event);
elf_status_t elf_queue_dequeue(elf_queue_t queue, elf_event_t * ref_event);

