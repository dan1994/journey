target remote | qemu-system-x86_64 -hda bin/os.bin -S -gdb stdio
add-symbol-file build/kernel/kernel.o -s .start 0x100000 -s .text 0x101000
set disassembly-flavor intel
layout asm
set confirm off
focus cmd
b *0x7c00
c