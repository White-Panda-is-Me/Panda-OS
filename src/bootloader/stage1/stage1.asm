org 0x7c00
bits 16

%define endl 0x0D, 0x0A
jmp short start
nop

; mlfs+ header goes here:
mlfs_bps: 						dw 512			; bytes per sector
mlfs_total_blocks: 				dw 20480
mlfs_magic_word:				dd 0x4D4C4653	; magic word "MLFS"
mlfs_max_root_dir_entries:		db 32
mlfs_reserved_blocks:			db 1
mlfs_spt:						db 63			; sector per track
mlfs_hpc:						db 16
mlfs_cylinders:					dw 7

start:
	; setting up stack and segment registers
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7C00
	sti

	mov [drive_num], dl

	call get_drive_param

	cmp dl, 0x80			; I hate mother fucking floppy disks, so i don't support this piece of shit
	jae .continue

	cli
	hlt

.continue:
	; reading the stage2 into memory
	push ax
	push bx
	push cx

	mov bx, STAGE2_LOAD_SEGMENT
	mov es, bx
	mov bx, STAGE2_LOAD_OFFSET
	mov ax, 1
	mov cl, 8
	call disk_read

	pop cx
	pop bx
	pop ax
	; load stage2.bin into address 0x0000:0500 (es:bx)

	;now jump to the stage2 address into the memory
	jmp STAGE2_LOAD_SEGMENT:STAGE2_LOAD_OFFSET

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

get_drive_param:
	; respect to registers
	pusha

	mov dl, [drive_num]
	xor di, di
	mov es, di
	mov ah, 08h

	int 13h
	jc .fucked_up

	mov bl, ch
	mov bh, cl
	shr bh, 6
	inc bx
	mov [mlfs_cylinders], bx		; total cylinders

	mov bl, cl
	and bl, 0x3F
	mov [mlfs_spt], bl				; sector per track

	inc dh
	mov [mlfs_hpc], dh			; heads per cylinder

	popa
	ret

.fucked_up:
	mov si, get_drive_param_err_msg
	call puts
	cli
	hlt

; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;
;   @param AX = LBA value to load into mem		;
;   @param ES:BX = address to load the file		;
;   @param CL = sector count to read		    ;
; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;
disk_read:
	push dx
	push cx
	; push di

	call lba2chs
	pop ax		; actually moving sector count from cx to ax

	mov dl, [drive_num]
	mov ah, 02h

	int 13h
	jc .fucked_up

	; pop di
	pop dx
	ret

.fucked_up:
	mov si, disk_read_err_msg
	call puts

	cli
	hlt
lba2chs:
	xor dx, dx
	div word [mlfs_spt]
	inc dl
	mov cl, dl		; sector number

	xor dx, dx
	div word [mlfs_hpc]
	mov dh, dl
	mov ch, al
	ret

drive_num: 					db 0
get_drive_param_err_msg:	db "Disk param fucked up", endl, 0
disk_read_err_msg:			db "Disk read fucked up", endl, 0
STAGE2_LOAD_OFFSET: 		equ 0x0500
STAGE2_LOAD_SEGMENT:		equ 0x0000

times 510 - ($ - $$) db 0
dw 0xAA55
