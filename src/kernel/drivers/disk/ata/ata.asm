; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;
; ATA read sectors (LBA mode)                                                 ;
;                                                                             ;
; @param RAX Logical Block Address of sector                                  ;
; @param CL  Number of sectors to read                                        ;
; @param RDI The address of buffer to put data obtained from disk             ;
;                                                                             ;
; @return bytes read                                                          ;
; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;

global ata_lba_read
ata_lba_read:
    [bits 64]
    push rbp
    mov rbp, rsp

    pushfq
    push rbx
    push rcx
    push rdx
    push rdi

    mov rax, [rbp + 8]
    and rax, 0x0FFFFFFF
    xor rcx, rcx
    mov rcx, [rbp + 12]
    mov rdi, [rbp + 16]

    mov rbx, rax         ; Save LBA in EBX

    mov rdx, 0x01F6      ; Port to send drive and bit 24 - 27 of LBA
    shr rax, 24          ; Get bit 24 - 27 in al
    or al, 11100000b     ; Set bit 6 in al for LBA mode
    out dx, al
    nop
    nop

    mov rdx, 0x01F2      ; Port to send number of sectors
    mov al, cl           ; Get number of sectors from CL
    out dx, al
    nop
    nop

    mov rdx, 0x1F3       ; Port to send bit 0 - 7 of LBA
    mov rax, rbx         ; Get LBA from EBX
    out dx, al
    nop
    nop

    mov rdx, 0x1F4       ; Port to send bit 8 - 15 of LBA
    mov rax, rbx         ; Get LBA from EBX
    shr rax, 8           ; Get bit 8 - 15 in AL
    out dx, al
    nop
    nop

    mov rdx, 0x1F5       ; Port to send bit 16 - 23 of LBA
    mov rax, rbx         ; Get LBA from EBX
    shr rax, 16          ; Get bit 16 - 23 in AL
    out dx, al
    nop
    nop

    mov rdx, 0x1F7       ; Command port
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
    mov rax, 256         ; to read 512 bytes = 1 sector
    xor bx, bx
    mov bl, cl           ; read CL sectors
    mul bx
    mov rcx, rax         ; ECX is counter for INSW
    mov rdx, 0x1F0       ; Data port, in and out
    rep insw             ; in to [RDI]
    jmp .finish

.error:
    mov rax, 0

.finish:
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    popfq

    mov rsp, rbp
    pop rbp
    ret
