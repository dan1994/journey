ORG 7c00h ; Set offset of addresses to where the bootloader is loaded
BITS 16 ; Work in 16 bit mode

; BIOS Parameter Block

; Some BIOSes will expect a BPB (BIOS Parameter Block). It has to start with a
; jmp and a nop. The jmp will go to an intermediate step that will do another
; jmp, this time setting the code segment to the correct value.
bpb:
    jmp short bpb_trampoline
    nop

times 33 db 0 ; Dummy content for BPB fields

bpb_trampoline:
    jmp 0:start ; Sets code segment to 0

; Entrypoint

; Calculate GDT segment descriptor offsets (for 32 bit mode)
CODE_SEGMENT equ gdt_code - gdt_start
DATA_SEGMENT equ gdt_data - gdt_start

start:
.setup_segments_and_stack:
    cli ; Clear interrupts

    ; Setup segments (DS, ES, SS)
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; Start stack where we are loaded
    mov sp, 7c00h

    sti ; Enable interrupts

.enter_protected_mode:
    cli

    lgdt[gdt_descriptor] ; Load global descriptor table

    ; Enable protected mode by setting cr0 lowest bit
    mov eax, cr0
    or eax, 1h
    mov cr0, eax

    jmp CODE_SEGMENT:start32 ; Jump to 32 bit code

; GDT

gdt_start:
gdt_null:
    dd 0h
    dd 0h

; Offset 0x8. Relevant to CS
gdt_code:
    dw 0ffffh ; Segment limit 0:15
    dw 0h ; Base 0:15
    db 0h ; Base 16:23
    db 10011100b ; Access
    db 11001111b ; {Flags, Segment limit 16:19}
    db 0h ; Base 24:31

; Offset 0x10. Relevant to DS, SS, ES, FS, GS
gdt_data:
    dw 0ffffh ; Segment limit 0:15
    dw 0h ; Base 0:15
    db 0h ; Base 16:23
    db 10010011b ; Access
    db 11001111b ; {Flags, Segment limit 16:19}
    db 0h ; Base 24:31

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32] ; Code under here is 32 bits
start32:
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

; Sector Padding

times 510 - ($ - $$) db 0 ; Fill sector
dw 0aa55h ; Add bootable sector signature (reversed because of endianity)
