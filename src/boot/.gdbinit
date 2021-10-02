target remote | qemu-system-x86_64 -hda ../../bin/boot/boot.bin -S -gdb stdio
set disassembly-flavor intel
layout asm
set confirm off
focus cmd
b *0x7c00
c
