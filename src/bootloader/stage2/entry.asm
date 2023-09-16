[bits 16]

section .entry
extern main
global entry
entry:
    [bits 16]
    ; setting up segment ans stack registers
    xor ax, ax
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x500
    sti

    mov ax, 0x0007
    int 10h

    mov si, msg1
    call puts
    cli
    hlt

	; reading the KERNEL into memory
	mov ah, 02h
	mov al, 08h
	mov ch, 00h
	mov cl, 09h
	mov dh, 00h
	mov bx, KERNEL_LOAD_SEGMENT
	mov es, bx
	mov bx, KERNEL_LOAD_OFFSET
	; load KERNEL.bin into address 0x1000 (es:bx)

	int 13h
    jc disk_read_err

    cli
    call EnableA20

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
    push ax
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
; puts function
puts:
	pusha
    mov ah, 0Eh
	jmp .loop

.loop:
	lodsb
	cmp al, 0
	je .done
    int 10h
	jmp .loop

.done:
	popa
    ret

; A20 Gate enable
EnableA20:
	pusha
    ; First, check if A20 is already enabled
    in al, 0x92
    test al, 2
    jnz A20AlreadyEnabled

    ; A20 is not enabled, so enable it
    cli

    ; Send the unlock sequence to the keyboard controller
    mov al, 0xd1
    out 0x64, al
    in al, 0x60
    test al, 2
    jnz $

    mov al, 0xdf
    out 0x60, al
    in al, 0x60
    test al, 2
    jnz $

    ; A20 is now enabled
	popa
    sti
    ret

A20AlreadyEnabled:
	popa
    ret

disk_read_err:
    mov si, kernel_fuck
    call puts
    cli
    hlt

drive_num:                   db 0
msg1:                        db "Stage2 Loaded successfully!", 0
kernel_fuck:                 db "kernel read fucked up! shit", 0
KERNEL_LOAD_OFFSET: 		 equ 0x7E00
KERNEL_LOAD_SEGMENT:		 equ 0x0000
