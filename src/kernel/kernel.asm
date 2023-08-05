bits 16

%define endl 0x0D, 0x0A

entry:
    cli
    mov ax, 0x1000
    mov es, ax
    mov ds, ax
    mov ss, ax
    sti

    mov si, msg1
    call puts
    
.halt:
    jmp .halt

puts:
    mov ah, 0Eh
	jmp .loop

.loop:
	lodsb
	cmp al, 0
	je .done
    int 10h
	jmp .loop

.done:
    ret

msg1: db "Kernel loaded successfully!", endl, 0
