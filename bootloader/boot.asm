ORG 0x7c00 ; Offset all labels by the address where the bootloader is loaded
BITS 16 ; Work in 16 bit mode

start:
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
