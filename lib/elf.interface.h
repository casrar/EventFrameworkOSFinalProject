elf_status_t; // a status or error code
elf_event_t; // an event
elf_loop_t; // an event loop handle
elf_handler_t; // an event handling function
elf_status_t handler(elf_loop_t loop, elf_event_t event);
elf_status_t elf_main(elf_handler_t handler);
elf_status_t elf_init(elf_loop_t* ref_loop, elf_handler_t handler);
elf_status_t elf_fini(elf_loop_t* ref_loop);
elf_status_t elf_send(elf_loop_t loop, elf_event_t event);