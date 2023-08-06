[bits 16]

section .text
    global entry
entry:
    [bits 16]
    ; setting up segment ans stack registers
    cli
    xor ax, ax
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x7E00
    sti

    mov si, msg1
    call puts
    cli

    ; now time to load the GDT
    lgdt [GDT_desc]

    ; A20 grate is already enabled so set the controll flag as 1
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 08h:Pmode

Pmode:
    [bits 32]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov edi, 0xB8000
    mov byte [edi], 'A'
    inc edi
    mov edi, 0x0000F4


.halt:
    jmp .halt

GDT:
    ; reservered
    dq 0

    ; Code segment 32 bit
    dw 0FFFFh
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0

    ; Data segment 32 bit
    dw 0FFFFh
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0

GDT_desc:
    dw GDT_desc - GDT - 1
    dd GDT

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

msg1:                        db "Stage2 Loaded successfully!", 0xD, 0xA, 0
Vid_mem:                     equ 0xB8000
