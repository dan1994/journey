[BITS 32] ; Code under here is 32 bits

section .start

global _start
extern initialize_global_variables
extern kernel_main
extern finalize_global_variables

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

.switch_to_cpp:
    call initialize_global_variables
    call kernel_main
    call finalize_global_variables

.infinite_loop:
    jmp $

; This code is going to precede the rest of our C code since it has to be loaded
; at address 0x100000. In order to avoid alignment issues with the C code that
; comes right after we make sure to align to a sector boundary.
; The rest of the assembly will be put into a seperate section at the end.
times 512 - ($ - $$) db 0