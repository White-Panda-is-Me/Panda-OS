[bits 64]

extern x64_ISR_Handler

%macro ISR_NOERR 1

global x64_ISR%1
x64_ISR%1:
    push 0
    push %1
    jmp common_isr

%endmacro

%macro ISR_ERR 1

global x64_ISR%1
x64_ISR%1:
    push %1
    jmp common_isr

%endmacro

%include "arch/x64/big_isr.inc"

common_isr:
    push rax
    push rcx
    push rdx
    push rbx
    push rbp
    push rsi
    push rdi

    mov rdi, rsp
    call x64_ISR_Handler

    pop rdi
    pop rsi
    pop rbp
    pop rbx
    pop rdx
    pop rcx
    pop rax
    add rsp, 16

    iretq


; section .bss

; regs:
; .rax:	resb 8
; .rcx:	resb 8
; .rdx:	resb 8
; .rbx:	resb 8
; .rsp:	resb 8
; .rbp:	resb 8
; .rsi:	resb 8
; .rdi:	resb 8

