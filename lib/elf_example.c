#include "elf_internal.h"
#include "elf_public.h"
#include <stdio.h>

struct {
    uint32_t other_loop;
    int32_t count;
} state_ping = { .other_loop = UINT32_MAX, .count = 0 };

elf_status_t handler_ping(uint32_t self_id, elf_event_t event) {

    if (state_ping.other_loop == UINT32_MAX) {
        state_ping.other_loop = event.value.loop_id;
        state_ping.count = 17;
    } else if (state_ping.count == 0) {
        return ELF_OK; // drop ball
    }

    printf("  ping\n");
    elf_send(state_ping.other_loop, elf_event_token());
    state_ping.count -= 1;

    return ELF_OK;
}


struct {
    uint32_t other_loop;
} state_pong = { .other_loop = UINT32_MAX };

elf_status_t handler_pong(uint32_t self_id, elf_event_t event) {
    if (state_pong.other_loop == UINT32_MAX) {
        state_pong.other_loop = event.value.loop_id;
        return ELF_OK;
    }

    printf("  pong\n");
    elf_send(state_ping.other_loop, elf_event_token());

    return ELF_OK;
}


elf_status_t handler_main(uint32_t self_id, elf_event_t event) {
    // assume, we are triggered at least once
    printf("handler_main(): triggered\n");
    
    uint32_t ping_id = UINT32_MAX;
    uint32_t pong_id = UINT32_MAX;

    elf_init(&ping_id, handler_ping);
    elf_init(&pong_id, handler_pong);

    elf_send(ping_id, elf_event_loop_id(pong_id));
    elf_send(pong_id, elf_event_loop_id(ping_id));

    return ELF_OK; // TODO: change to ELF_OK
}


int main() {
    printf("gu");
    elf_main(handler_main); // assume, this blocks main thread
    return 0;
}
