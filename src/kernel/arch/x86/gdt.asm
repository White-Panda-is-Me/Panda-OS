[bits 64]

global x86_GDT_Load
x86_GDT_Load:
    mov rax, rdi
    lgdt [rax]

    mov rax, rsi
    push rax
    push .load_cs
    retfq

.load_cs:
    mov rax, rdx
    mov ds, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov es, ax

    ret
