[bits 64]

global x64_GDT_Load
x64_GDT_Load:
	push rax

    lgdt [rdi]

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

	pop rax
    ret
