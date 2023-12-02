[bits 32]

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
    pusha       ; push some shit regs: eax, ecx, edx, ebx, esp, ebp, esi, edi

    push esp
    call x86_ISR_Handler
    add esp, 4
    popa
    add esp, 8
    iret
