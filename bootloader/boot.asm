ORG 0x0 ; Set offset of addresses to 0 and use segment registers instead
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
    jmp 0x7c0:start ; Sets code segment to where the bootloader is loaded

; Main Code

start:
    cli ; Clear interrupts

    ; Setup segments
    mov ax, 0x7c0
    mov ds, ax ; DS = 0x7c00
    mov es, ax ; ES = 0x7c00

    ; Setup stack
    mov ax, 0x0
    mov ss, ax ; SS = 0x0
    mov sp, 0x7c00

    sti ; Enable interrupts

    mov si, message
    call print
    jmp $ ; Infinite loop

; @arg - si - point to the string to print
print:
    mov bx, 0 ; Set foreground color
    mov ah, 0eh ; Set interrupt to print
.loop:
    lodsb ; al = si++
    cmp al, 0 ; Did we reach null?
    je .done
    int 0x10
    jmp .loop
.done:
    ret

message: db 'Hello, World!', 0
times 510 - ($ - $$) db 0 ; Fill sector
dw 0xAA55 ; Add bootable sector signature (reversed because of endianity)
