target remote | qemu-system-i386 -drive file=../../bin/boot/boot.bin,format=raw,index=0,media=disk -S -gdb stdio
set disassembly-flavor intel
layout asm
set confirm off
focus cmd
b *0x7c00
c
