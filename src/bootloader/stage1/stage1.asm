org 0x7c00
bits 16

%define endl 0x0D, 0x0A

entry:
	; setting up stack and segment registers
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7C00
	sti

	; set up video mode
	xor ah, ah
	mov al, 03h
	int 10h

	; expect drive number in dl
	mov [drive_num], dl
	; enable a20 gate so we can use more than 1MB of RAM
	call EnableA20

	; reset the disk
	mov ah, 0
	int 13h

	mov si, msg
	call puts

	; reading the stage2 into memory
	mov dl, [drive_num]
	mov ah, 02h
	mov al, 07h
	mov ch, 00h
	mov cl, 02h
	mov dh, 00h
	mov bx, STAGE2_LOAD_SEGMENT
	mov es, bx
	mov bx, STAGE2_LOAD_OFFSET
	; ; load stage2.bin into address 0x0000:7E00 (es:bx)

	int 13h
	jc disk_read_err
	
	; reading into memory done!
	mov si, done_reading_disk
	call puts

	; again reset the disk
	mov ah, 0
	int 13h

	; reading the kernel into memory
	mov dl, [drive_num]
	mov ah, 02h
	mov al, 01h
	mov ch, 00h
	mov cl, 08h
	mov dh, 01h
	mov bx, KERNEL_LOAD_SEGMENT
	mov es, bx
	mov bx, KERNEL_LOAD_OFFSET
	; load kernel.bin into address 0x1000:0000 (es:bx)

	int 13h
	jc disk_read_err

	;now jump to the stage2 address into the memory
	jmp KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET

	cli
	hlt

; puts function
puts:
	pusha
    mov ah, 0Eh
	jmp .loop

.loop:
	lodsb
	cmp al, 0
	je .done
    int 10h
	jmp .loop

.done:
	popa
    ret

; A20 Gate enable
EnableA20:
	pusha
    ; First, check if A20 is already enabled
    in al, 0x92
    test al, 2
    jnz A20AlreadyEnabled

    ; A20 is not enabled, so enable it
    cli

    ; Send the unlock sequence to the keyboard controller
    mov al, 0xd1
    out 0x64, al
    in al, 0x60
    test al, 2
    jnz $

    mov al, 0xdf
    out 0x60, al
    in al, 0x60
    test al, 2
    jnz $

    ; A20 is now enabled
	popa
    sti
    ret

A20AlreadyEnabled:
	popa
    ret

; error handling
disk_read_err:
	mov si, reading_disk_err_msg
	call puts

drive_num: 					db 0
msg: 						db "Welcome to SOMEONE OS", endl, 0
reading_disk_err_msg:		db "Error Reading Sectors into memory", endl, 0
done_reading_disk:			db "Reading sectors into memory done!", endl, 0
STAGE2_LOAD_OFFSET: 		equ 0x7E00
STAGE2_LOAD_SEGMENT:		equ 0x0000

KERNEL_LOAD_OFFSET: 		equ 0x0000
KERNEL_LOAD_SEGMENT:		equ 0x1000

times 510 - ($ - $$) db 0
dw 0xAA55