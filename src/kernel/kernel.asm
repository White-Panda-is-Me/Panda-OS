[bits 32]

section .entry

extern kmain
global kentry
kentry:
    mov esp, 0x1000
    jmp kmain
    pop ax

.halt:
    jmp .halt
