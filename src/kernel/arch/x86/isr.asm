[bits 64]

extern x86_ISR_Handler

%macro ISR_NOERR 1

global x86_ISR%1
x86_ISR%1:
    push 0
    push %1
    jmp common_isr

%endmacro

%macro ISR_ERR 1

global x86_ISR%1
x86_ISR%1:
    push %1
    jmp common_isr

%endmacro

%include "arch/x86/big_isr.inc"

common_isr:
    push rax       ; push some shit regs: rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi
    push rcx
    push rdx
    push rbx
    push rsp
    push rbp
    push rsi
    push rdi

    push rsp
    call x86_ISR_Handler
    add rsp, 8

    pop rdi
    pop rsi
    pop rbp
    pop rsp
    pop rbx
    pop rdx
    pop rcx
    pop rax       ; push some shit regs: rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi

    add rsp, 16
    iretq
