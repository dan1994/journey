[BITS 32]

section .asm

global load_page_directory

; Load a new page directory. Set CR3 to the address of the page directory.
;
; @param ebp + 8 - The address of the page directory.
load_page_directory:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]
    mov cr3, eax
    pop ebp
    ret

global enable_paging

; Enable the paging mechanism. Set a flag in CR0 to do so.
enable_paging:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 80000000h
    mov cr0, eax
    pop ebp
    ret
