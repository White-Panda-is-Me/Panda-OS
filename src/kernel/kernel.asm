[bits 32]

section .kentry

global kentry
kentry:
    mov ax, 0x1000
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov gs, ax
    mov fs, ax

.halt:
    jmp .halt

msg: db "h", 0