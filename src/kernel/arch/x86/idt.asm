;
; void __attribute__((cdecl)) x86_GDT_Load(GDT_Descriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);
;
[bits 32]
global x86_IDT_Load
x86_IDT_Load:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    lidt [eax]

    mov esp, ebp
    pop ebp
    ret