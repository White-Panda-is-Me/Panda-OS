;
; void __attribute__((cdecl)) x86_GDT_Load(GDT_Descriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);
;
[bits 32]
global x86_GDT_Load
x86_GDT_Load:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    lgdt [eax]

    mov eax, [ebp + 12]
    push eax
    push .load_cs
    retf

.load_cs:
    mov ax, [ebp + 16]
    mov ds, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov es, ax

    mov esp, ebp
    pop ebp
    ret