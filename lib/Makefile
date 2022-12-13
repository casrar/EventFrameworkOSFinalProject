build:
	@gcc -std=c11 -pedantic elf_engine.c elf_queue.c elf_example.c -o example -lpthread

run:
	@./example

clean:
	@rm -f example

dev: build run clean