# Event Loop Framework (ELF)

## Compilation

- this code compiles with:

    *gcc -std=c11 -pedantic elf_engine.c elf_queue.c elf_example.c -o example*

- you should write a **Makefile** though

## Initial Steps

- implement the multi-threaded unbounded `elf_queue_t` first
  - `elf_dequeue()` operation blocks if queue is empty 
  - `elf_enqeue()` operation returns `ELF_FULL` if queue is full
  - you need to use POSIX thread functions here

- modify `elf_loop_routine()` to dequeue properly
  - make sure that the main event loop is triggered at least once

- implement the `elf_init()` operation
  - creates a new loop or returns ELF_ERROR
  - make sure you lock the global `elf_loops` array
  
- implement the `elf_send()` operation
  - you should be able to send a message to a loop using the loop's id
  - a simple test may be to self-trigger the main loop
  - sending messages to other loops requires making this operation thread-safe

- get the ping-pong communication running
  - once it is running try to implement 
