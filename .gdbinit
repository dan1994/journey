target remote | qemu-system-i386 -hda bin/os.bin -S -gdb stdio
add-symbol-file build/kernel/kernel.o -s .start 0x100000 -s .text 0x101000 -s .rodata 0x102000 -s .bss 0x103000 -s .asm 0x104000 -s .ctors 0x105000 -s .dtors 0x106000
set disassembly-flavor intel
layout asm
set confirm off
focus cmd
b *0x7c00
c
