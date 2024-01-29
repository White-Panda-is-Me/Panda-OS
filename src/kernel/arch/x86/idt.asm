;
; void __attribute__((cdecl)) x86_GDT_Load(GDT_Descriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);
;
[bits 64]
global x86_IDT_Load
x86_IDT_Load:
    push rbp
    mov rbp, rsp

    mov rax, [rbp + 8]
    lidt [rax]

    mov rsp, rbp
    pop rbp
    ret
