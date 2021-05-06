[BITS 32] ; Code under here is 32 bits

global _start

CODE_SEGMENT equ 8h
DATA_SEGMENT equ 10h

_start:
.setup_segments_and_stack:
    mov ax, DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ebp, 0200000h
    mov esp, ebp

.enable_a20_line: ; Allows addressing more than 1 MB
    in al, 0x92
    or al, 2
    out 0x92, al

.infinite_loop:
    jmp $
