[bits 16]

section .entry
extern main
extern putc
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

    jmp KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET

    hlt

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

msg1:                        db "Stage2 Loaded successfully!", 0
KERNEL_LOAD_OFFSET: 		 equ 0x0000
KERNEL_LOAD_SEGMENT:		 equ 0x1000
