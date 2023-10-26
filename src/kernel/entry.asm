[bits 32]

section .entry
extern kmain

global kentry
kentry:
    mov esp, stack_top
    mov ebp, stack_top
    call kmain

.halt:
    jmp .halt

section .bss
stack_bottom:
    resb 0x10000        ;stack size
stack_top: