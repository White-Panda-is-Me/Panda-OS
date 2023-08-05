org 0x7c00
bits 16

%define endl 0x0D, 0x0A

entry:
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7C00
	sti

	mov [drive_num], dl

	call EnableA20

	mov ah, 0
	int 13h

	mov si, msg
	call puts

	mov dl, [drive_num]
	mov ah, 02h
	mov al, 01h
	mov ch, 00h
	mov cl, 02h
	mov dh, 00h
	mov bx, KERNEL_LOAD_SEGMENT
	mov es, bx
	mov bx, KERNEL_LOAD_OFFSET

	int 13h
	jc disk_read_err
	
	mov si, done_reading_disk
	call puts

	jmp KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET

.halt:
    jmp .halt

; ; ; ; ; ; ; ; ; ; ; ;
puts:
    mov ah, 0Eh
	jmp .loop

.loop:
	lodsb
	cmp al, 0
	je .done
    int 10h
	jmp .loop

.done:
    ret
; ; ; ; ; ; ; ; ; ; ; ;

; ; ; ; ; ; ; ; ; ; ; ;
EnableA20:
    ; First, check if A20 is already enabled
    in al, 0x92         ; Read the system control port B
    test al, 2          ; Test the A20 enable bit (bit 1)
    jnz A20AlreadyEnabled   ; Jump if A20 is already enabled

    ; A20 is not enabled, so enable it
    cli                 ; Disable interrupts

    ; Send the unlock sequence to the keyboard controller
    mov al, 0xd1        ; Command to write to the input buffer
    out 0x64, al
    in al, 0x60         ; Wait for the keyboard controller to be ready
    test al, 2
    jnz $                  ; Keep waiting if the controller is not ready

    mov al, 0xdf        ; A20 enable bit mask (bit 1 set)
    out 0x60, al        ; Write the A20 enable bit to the input buffer
    in al, 0x60         ; Wait for the keyboard controller to be ready
    test al, 2
    jnz $                  ; Keep waiting if the controller is not ready

    ; A20 is now enabled
    sti                 ; Enable interrupts
    ret

A20AlreadyEnabled:
    ret
; ; ; ; ; ; ; ; ; ; ; ;

; ; ; ; ; ; ; ; ; ; ; ;
disk_read_err:
	mov si, reading_disk_err_msg
	call puts
; ; ; ; ; ; ; ; ; ; ; ;

drive_num: 					db 0
msg: 						db "Welcome to MAMMAD OS", endl, 0
reading_disk_err_msg:		db "Error Reading Sectors into memory", endl, 0
done_reading_disk:			db "Reading sectors into memory done!", endl, 0
KERNEL_LOAD_OFFSET: 		equ 0x0000
KERNEL_LOAD_SEGMENT:		equ 0x1000


times 510 - ($ - $$) db 0
dw 0xAA55