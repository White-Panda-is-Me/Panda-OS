[bits 64]
global x64_IDT_Load
x64_IDT_Load:
    lidt [rdi]
    ret
