; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;
; ATA read sectors (LBA mode)                                                 ;
;                                                                             ;
; @param EAX Logical Block Address of sector                                  ;
; @param CL  Number of sectors to read                                        ;
; @param EDI The address of buffer to put data obtained from disk             ;
;                                                                             ;
; @return bytes read                                                          ;
; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;

global ata_lba_read
ata_lba_read:
    [bits 32]
    push ebp
    mov ebp, esp

    pushfd
    push ebx
    push ecx
    push edx
    push edi

    mov eax, [ebp + 8]
    and eax, 0x0FFFFFFF
    xor ecx, ecx
    mov ecx, [ebp + 12]
    mov edi, [ebp + 16]

    mov ebx, eax         ; Save LBA in EBX

    mov edx, 0x01F6      ; Port to send drive and bit 24 - 27 of LBA
    shr eax, 24          ; Get bit 24 - 27 in al
    or al, 11100000b     ; Set bit 6 in al for LBA mode
    out dx, al
    nop
    nop

    mov edx, 0x01F2      ; Port to send number of sectors
    mov al, cl           ; Get number of sectors from CL
    out dx, al
    nop
    nop

    mov edx, 0x1F3       ; Port to send bit 0 - 7 of LBA
    mov eax, ebx         ; Get LBA from EBX
    out dx, al
    nop
    nop

    mov edx, 0x1F4       ; Port to send bit 8 - 15 of LBA
    mov eax, ebx         ; Get LBA from EBX
    shr eax, 8           ; Get bit 8 - 15 in AL
    out dx, al
    nop
    nop

    mov edx, 0x1F5       ; Port to send bit 16 - 23 of LBA
    mov eax, ebx         ; Get LBA from EBX
    shr eax, 16          ; Get bit 16 - 23 in AL
    out dx, al
    nop
    nop

    mov edx, 0x1F7       ; Command port
    mov al, 0x20         ; Read with retry.
    out dx, al


    nop
    nop
.still_going:
    in al, dx
    test al, 00001000b
    jnz .lets_go

    jmp .still_going

.lets_go:
    mov eax, 256         ; to read 512 bytes = 1 sector
    xor bx, bx
    mov bl, cl           ; read CL sectors
    mul bx
    mov ecx, eax         ; ECX is counter for INSW
    mov edx, 0x1F0       ; Data port, in and out
    rep insw             ; in to [EDI]
    jmp .finish

.error:
    mov eax, 0

.finish:
    pop edi
    pop edx
    pop ecx
    pop ebx
    popfd

    mov esp, ebp
    pop ebp
    ret