[bits 16]

section .entry
extern main
global entry
entry:
    pusha
    [bits 16]
    ; setting up segment ans stack registers
    cli
    xor ax, ax
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x7E00
    sti

    ; mov ah, 0
    ; int 13h

    ; expect drive number in dl
	mov [drive_num], dl

	; reading the KERNEL into memory
	mov dl, [drive_num]
	mov ah, 02h
	mov al, 08h
	mov ch, 00h
	mov cl, 09h
	mov dh, 00h
	mov bx, KERNEL_LOAD_SEGMENT
	mov es, bx
	mov bx, KERNEL_LOAD_OFFSET
	; ; load KERNEL.bin into address 0x10000 (es:bx)

	int 13h
    jc disk_read_err
    xor al, al
    push ax

    cli

    ; now time to load the GDT
    lgdt [GDT_desc]

    ; A20 grate is already enabled so set the controll flag as 1
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 08h:Pmode

[bits 32]
Pmode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    call main

    popa
    jmp 0x8:KERNEL_LOAD_OFFSET

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

disk_read_err:
    cli
    hlt

drive_num:                   db 0
msg1:                        db "KERNEL Loaded successfully!", 0
KERNEL_LOAD_OFFSET: 		 equ 0x1000
KERNEL_LOAD_SEGMENT:		 equ 0x0000
