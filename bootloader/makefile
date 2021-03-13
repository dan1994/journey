.PHONY: all
all: run

run: compile
	qemu-system-x86_64 -hda boot.bin

compile: boot.bin

boot.bin: boot.asm
	nasm -f bin $< -o $@