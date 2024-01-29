[bits 64]

extern kmain

global x86_GDT_Load
x86_GDT_Load:
    mov rax, [rbp + 12]
    lgdt [rax]

    mov rax, [rbp + 20]
    push rax
    push .load_cs
    retf

.load_cs:
	mov rax, [rbp + 28]
	mov ds, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax
	mov es, ax

	call kmain

	cli
	hlt
