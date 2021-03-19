ORG 0h ; Set offset of addresses to 0 and use segment registers instead
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
    jmp 7c0h:start ; Sets code segment to where the bootloader is loaded

; Main Code

start:
    cli ; Clear interrupts

    ; Setup segments
    mov ax, 7c0h
    mov ds, ax ; DS = 0x7c00
    mov es, ax ; ES = 0x7c00

    ; Setup stack
    mov ax, 0h
    mov ss, ax ; SS = 0x0
    mov sp, 7c00h

    sti ; Enable interrupts

    call main
    jmp $ ; Infinite loop

main:
    ; read_sectors(buffer, offset=2, number=1)
    mov bx, buffer
    mov cl, 2h
    mov al, 1h
    call read_sectors

    ; if(failed) goto .done
    cmp ax, 0h
    jnz .done

    ; print(buffer)
    mov si, buffer
    call print
.done:
    ret

; @arg - bx - pointer to buffer
; @arg - cl - Sector offset
; @arg - al - Number of sectors
; @return - ax - 0 iff success
read_sectors:
    mov ah, 2h ; Set interrupt to read
    mov ch, 0h ; Cylinder low eight bits
    mov dh, 0h ; Head number
    int 13h
    jnc .done
.error:
    mov si, .error_message
    call print
    mov ax, -1
    ret
.done:
    mov ax, 0
    ret
.error_message: db 'Failed to read sectors', 0

; @arg - si - point to the string to print
print:
    mov bx, 0h ; Set foreground color
    mov ah, eh ; Set interrupt to print
.loop:
    lodsb ; al = si++
    cmp al, 0h ; Did we reach null?
    je .done
    int 10h
    jmp .loop
.done:
    ret

times 510 - ($ - $$) db 0 ; Fill sector
dw AA55h ; Add bootable sector signature (reversed because of endianity)

buffer:
