build:
	@gcc -std=c11 -pedantic elf_example.c -L. elf_lib.a -lpthread -o example

run:
	@./example

clean:
	@rm -f example

dev: build run clean