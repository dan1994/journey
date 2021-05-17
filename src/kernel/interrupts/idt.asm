section .asm

global load_idt

; Load the IDT info into the IDT Register
;
; @param ebp + 8 - Address of IDT register structure
load_idt:
    push ebp
    mov ebp, esp

    mov ebx, [ebp + 8]
    lidt [ebx]

    pop ebp
    ret
