global x64_outb
x64_outb:
    [bits 64]
	push rbp
	mov rbp, rsp
	push rax
	push rdx

	mov rdx, rdi
    mov rax, rsi
    out dx, al

	pop rdx
	pop rax
	mov rsp, rbp
	pop rbp
    ret

global x64_outl
x64_outl:
    [bits 64]
	push rbp
	mov rbp, rsp
	push rax
	push rdx

	mov rdx, rdi
    mov rax, rsi
    out dx, eax

	pop rdx
	pop rax
	mov rsp, rbp
	pop rbp
    ret

global x64_inb
x64_inb:
    [bits 64]
	push rbp
	mov rbp, rsp
	push rdx

	mov rdx, rdi
	xor rax, rax
    in al, dx

	pop rdx
	mov rsp, rbp
	pop rbp
    ret

global x64_inw
x64_inw:
    [bits 64]
	push rbp
	mov rbp, rsp
	push rdx

	mov rdx, rdi
	xor rax, rax
    in ax, dx

	pop rdx
	mov rsp, rbp
	pop rbp
    ret

global x64_inl
x64_inl:
    [bits 64]
	push rbp
	mov rbp, rsp
	push rdx

	mov rdx, rdi
	xor rax, rax
    in eax, dx

	pop rdx
	mov rsp, rbp
	pop rbp
    ret

global x64_insw
x64_insw:
	[bits 64]
    push rbp
    mov rbp, rsp

    push rcx

    mov rcx, rdx
	mov rdx, rsi
    insw

    pop rcx

    mov esp, ebp
    pop rbp

    ret

global panic
panic:
    cli
    hlt


global fuck
fuck:
    int 0x1
    ret
