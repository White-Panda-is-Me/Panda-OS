[bits 16]

section .entry

extern main
global entry
entry:
    [bits 16]
    cli
    ; setting up segment ans stack registers
    xor ax, ax
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov esp, stack_top

    call EnableA20

    ; now time to load the GDT
    lgdt [GDT_desc]

    ; set the controll flag as 1
    mov eax, cr0
    or al, 1
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

    ; 16-bit code segment
    dw 0FFFFh
    dw 0
    db 0
    db 10011010b
    db 00001111b
    db 0

    ; 16-bit data segment
    dw 0FFFFh
    dw 0
    db 0
    db 10010010b
    db 00001111b
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

section .bss
stack_bottom:
    resb 0x10000
stack_top: