global x86_outb
x86_outb:
    [bits 64]

    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al

    ret

global x86_inb
x86_inb:
    [bits 64]

    mov dx, [esp + 4]
    in al, dx

    ret

global x86_inw
x86_inw:
    [bits 64]

    mov dx, [esp + 4]
    in ax, dx

    ret

global x86_insw
x86_insw:
	[bits 64]
    push rbp
    mov ebp, esp

    push rdi
    push rcx
    push rdx

    mov edi, [ebp + 8]
    mov ecx, [ebp + 12]
    mov edx, [ebp + 16]
    insw

    pop rdx
    pop rcx
    pop rdi

    mov esp, ebp
    pop rbp

    ret

global panic
panic:
    cli
    hlt

