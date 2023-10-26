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
mlfs_reserved_blocks:			db 2
mlfs_spt:						db 0 			; sector per track
mlfs_hpc:						db 0
mlfs_cylinders:					dw 0

start:
	; setting up stack and segment registers
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov esp, 0x7c00
	sti

	mov [drive_num], dl

	call get_drive_param

	cmp dl, 0x80			; I hate fucking floppy disks, so i don't support this piece of shit
	jae .continue

	cli
	hlt

.continue:
	; get the info about stage2
	mov bx, 0
	mov es, bx
	mov bx, file_block_start
	mov ax, 1			; second sector contains startBlock of each entry inside root directory
	mov cl, 1
	call disk_read

	; now we read stage2 into memory
	mov si, stage2_name
	mov di, STAGE2_LOAD_SEGMENT
	mov es, di
	mov di, STAGE2_LOAD_OFFSET
	mov cx, 10
	call load_file

	; it's time for common files
	; mov si, common_name
	; mov di, COMMON_LOAD_SEGMENT
	; mov es, di
	; mov di, COMMON_LOAD_OFFSET
	; mov cx, 10
	; call load_file
	jmp STAGE2_LOAD_SEGMENT:STAGE2_LOAD_OFFSET


; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;
;   @param DS:SI = name of the file				;
;   @param CX = size of the file name			;
;	@param ES:DI = address to load the file		;
;												;
;	@return none							    ;
; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;
load_file:
	push di
	push dx
	xor dx, dx

.search_file:
	push si
	push cx
	push es
	mov ax, [bx]
	xor bx, bx
	mov es, bx
	mov bx, file_entry_info_block
	mov cl, 1
	call disk_read
	pop es
	pop cx
	pop si

	mov di, file_entry_info_block.name
	repe cmpsb

	jz .found_file

.next_block:
	add bx, 4
	add dx, 4
	cmp dx, 512
	jae .file_not_found

	jmp .search_file

.found_file:
	mov dx, ds
	mov es, dx
	pop dx
	pop di

	mov bx, di
	mov ax, [file_entry_info_block.BlockStart]
	mov cl, [file_entry_info_block.FileSizeInSector]
	call disk_read

	ret

.file_not_found:
	pop dx
	pop di
	; the file name is already inside si
	call puts
	mov si, file_not_found_msg
	call puts

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
	mov [mlfs_hpc], dh				; heads per cylinder

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
;   @param CL = sector count to read			;
;												;	
;	@return none							    ;
; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;
disk_read:
	push dx
	push cx

	call lba2chs
	pop ax		; actually moving sector count from cx to ax

	mov dl, [drive_num]
	mov ah, 02h

	int 13h
	jc .fucked_up

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
get_drive_param_err_msg:	db "Disk param failed", endl, 0
disk_read_err_msg:			db "Disk read failed", endl, 0
file_not_found_msg:			db " not found!", endl, 0
STAGE2_LOAD_OFFSET: 		equ 0x0500
STAGE2_LOAD_SEGMENT:		equ 0x0000

stage2_name:				db 'stage2.bin', 0

file_entry_info_block:			; F stands for file
	; Inode:
		.inode_num:			dd 0
		.flags:				db 0
		.CreatedTime:		dw 0
		.CreatedDate:		dw 0
	; file:
		.name:				times 120 db 0
		.BlockStart:		dd 0
		.FileSize:			dd 0
		.FileSizeInSector:	dd 0

times 510 - ($ - $$) db 0
dw 0xAA55

file_block_start: