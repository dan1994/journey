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
    db 10011010b ; Access
    db 11001111b ; {Flags, Segment limit 16:19}
    db 0h ; Base 24:31

; Offset 0x10. Relevant to DS, SS, ES, FS, GS
gdt_data:
    dw 0ffffh ; Segment limit 0:15
    dw 0h ; Base 0:15
    db 0h ; Base 16:23
    db 10010010b ; Access
    db 11001111b ; {Flags, Segment limit 16:19}
    db 0h ; Base 24:31

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]

; Load the kernel
start32:
    mov eax, 1 ; Sector 1
    mov ecx, 100 ; 100 Sectors
    mov edi, 100000h ; At address
    call ata_lba_read

    jmp CODE_SEGMENT:100000h

; Read from disk
; ATA - the specification for disk interface (like in SATA)
; LBA - Logical Block Addressing
;
; EAX - Sector offset (also referred to as LBA)
; ECX - Sector amount
; EDI - Load to
ata_lba_read:
    ; Override the uppermost nibble of the offset which is used to set LBA mode
    ; and the drive number (0 in this case)
    and eax, 0fffffffh
    or eax, 0e0000000h

    mov ebx, eax ; Save the offset for later since eax is needed

    ; Write the sector amount to IO address 0x1F2
    mov dx, 1f2h
    mov eax, ecx
    out dx, al

    mov eax, ebx ; Restore the offset to eax

    ; Write the offset to 4 consecutive IO addresses
.write_offset:
    inc dx
    out dx, al
    shr eax, 8
    cmp dx, 1f6h
    jnz .write_offset

    ; Set command to "Read with retry"
    inc dx
    mov al, 20h
    out dx, al

    ; Outer loop that counts sectors
.read_sector:
    push ecx ; Backup loop counter as it will be used in inner loop

    ; Busy wait until data is available
.wait_for_data:
    in al, dx
    test al, 8
    jz .wait_for_data

    mov dx, 0x1f0 ; Setup IO register to where read data is stored

    ; Inner loop that reads all bytes of the sector
    ; These two instructions say repeat insw 256 times, where insw reads from
    ; the IO port specifies in DX into DI and then increments DI
    mov ecx, 256
    rep insw

    ; Restore the outer loop counter and repeat until it reaches 0
    pop ecx
    loop .read_sector

    ret

; Sector Padding

times 510 - ($ - $$) db 0 ; Fill sector
dw 0aa55h ; Add bootable sector signature (reversed because of endianity)
