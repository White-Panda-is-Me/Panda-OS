[bits 32]

section .entry
extern kmain

global kentry
kentry:
    [bits 32]
    ; xor eax, eax
    ; mov ax, 0x10
    ; mov ss, ax
    ; mov es, ax
    ; mov ds ,ax

    mov esp, 0x100000

    ; mov edi, 0xB8000
    ; mov byte [edi], 'A'
    call kmain
    ; call kmain

    hlt

; section .bss
; stack_top:
    ; resb 0x10000        ;stack size