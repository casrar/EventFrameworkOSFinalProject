/*

  Public Interface to Event Loop Framework (ELF)

*/

#pragma once
#include <stdint.h>

#define ELF_CAP_LOOPS 16

enum elf_status_e {
    ELF_OK,
    ELF_ERROR,
    ELF_EMPTY,
    ELF_FULL,
};
typedef enum elf_status_e elf_status_t;


enum elf_type_e {
    ELF_TOKEN,
    ELF_LOOP_ID,
    ELF_PTR,
    ELF_INT32,
};
typedef enum elf_type_e elf_type_t;


typedef struct  {
    elf_type_t type;
    union {
        uint32_t loop_id;
        void * ptr;
        int32_t int32;
    } value;
} elf_event_t;


typedef elf_status_t (*elf_handler_t)(uint32_t, elf_event_t);

elf_status_t elf_main(elf_handler_t handler);
elf_status_t elf_init(uint32_t *ref_loop_id, elf_handler_t handler);
elf_status_t elf_fini(uint32_t loop_id);

elf_status_t elf_send(uint32_t loop_id, elf_event_t event);


inline elf_event_t elf_event_token() {
    elf_event_t event = {.type = ELF_TOKEN, .value.int32 = 0};
    return event;
}

inline elf_event_t elf_event_loop_id(uint32_t loop_id) {
    elf_event_t event = {.type = ELF_LOOP_ID, .value.loop_id = loop_id};
    return event;
}

inline elf_event_t elf_event_ptr(void * ptr) {
    elf_event_t event = {.type = ELF_PTR, .value.ptr = ptr};
    return event;
}

inline elf_event_t elf_event_int32(int32_t loop_id) {
    elf_event_t event = {.type = ELF_INT32, .value.int32 = loop_id};
    return event;
}





