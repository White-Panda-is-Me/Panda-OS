global x86_outb
x86_outb:
    [bits 32]

    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al

    ret

global x86_inb
x86_inb:
    [bits 32]

    mov dx, [esp + 4]
    in al, dx

    ret

global x86_inw
x86_inw:
    [bits 32]

    mov dx, [esp + 4]
    in ax, dx

    ret

global x86_insw
x86_insw:
    push ebp
    mov ebp, esp

    push edi
    push ecx
    push edx

    mov edi, [ebp + 8]
    mov ecx, [ebp + 12]
    mov edx, [ebp + 16]
    insw

    pop edx
    pop ecx
    pop edi

    mov esp, ebp
    pop ebp

    ret

%macro LinearToSegOffset 4

    mov %3, %1      ; linear address to eax
    shr %3, 4
    mov %2, %4
    mov %3, %1      ; linear address to eax
    and %3, 0xf

%endmacro

EnterRealMode:
    [bits 32]
    jmp dword 18h:.pmode16         ; 1 - jump to 16-bit protected mode segment

.pmode16:
    [bits 16]
    ; 2 - disable protected mode bit in cr0
    mov eax, cr0
    and eax, ~1
    mov cr0, eax

    ; 3 - jump to real mode
    jmp dword 00h:.rmode

.rmode:
    ; 4 - setup segments
    mov ax, 0
    mov ds, ax
    mov ss, ax

    ; 5 - enable interrupts
    sti

    ret

EnterProtectedMode:
    cli

    ; 4 - set protection enable flag in CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; 5 - far jump into protected mode
    jmp dword 08h:.pmode

.pmode:
    ; we are now in protected mode!
    [bits 32]
    
    ; 6 - setup segment registers
    mov ax, 0x10
    mov ds, ax
    mov ss, ax

    ret

;
; int Memdetect(Mem_Map* map, uint32_t* contID);
;
global MemDetect
MemDetect:
    [bits 32]
    push ebp
    mov ebp, esp

    push edx
    push ecx
    push ebx
    push di
    push si

    call EnterRealMode
    [bits 16]

    mov edx, 0x534D4150             ; "SMAP" - signature
    xor ebx, ebx                    ; clear ebx
    LinearToSegOffset [ebp + 8], es, edi, di
    LinearToSegOffset [ebp + 12], ds, esi, si
    mov ebx, ds:[si]
    ; mov [es:di + 20], dword 0
    mov eax, 0xE820                 ; set interrupt (ax=E820h)
    mov ecx, 24                     ; reading 24 bytes
    int 15h                         ; obvious as I said
    jc .error                       ; function is not supported
    mov edx, 0x534D4150             ; edx is fucked up and has to be set again
    cmp edx, eax                    ; if eax isn't equal to edx then 
    jne .error                      ; it means it is fucked up somehow

    mov ds:[si], ebx

    jmp .done


.error:
    mov ecx, -1

.done:
    call EnterProtectedMode
    [bits 32]

    mov eax, ecx
    
    pop si
    pop di
    pop ebx
    pop ecx
    pop edx

    mov esp, ebp
    pop ebp

    clc
    ret

global panic
panic:
    cli
    hlt

global check_long_mode
check_long_mode:
    mov eax, 0x80000000    ; implicit argument for cpuid
    cpuid                  ; get highest supported argument
    cmp eax, 0x80000001    ; it needs to be at least 0x80000001
    jb .no_long_mode       ; if it's less, the CPU is too old for long mode

    ; use extended info to test if long mode is available
    mov eax, 0x80000001    ; argument for extended processor info
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    jz .no_long_mode       ; If it's not set, there is no long mode
    mov eax, 1
    ret

.no_long_mode:
    mov eax, 0
    ret