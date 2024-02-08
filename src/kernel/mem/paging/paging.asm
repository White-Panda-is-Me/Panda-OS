; bits 64

; section .bss
; align 4096
; p4_table:
;     resb 4096
; p3_table:
;     resb 4096
; p2_table:
;     resb 4096

; setup_tables:
;     mov rax, p3_table
    

; global enable_paging
; enable_paging:
; 	push rbp
; 	mov rbp, rsp

; 	mov rax, cr0
; 	and rax, ~(1 << 31)
; 	mov cr0, rax

; 	mov rax, cr4
; 	or rdx, (1 << 5)
; 	mov cr4, rax

; 	mov ecx, 0xC0000080
; 	rdmsr
; 	or ecx, (1 << 8)
; 	wrmsr	

; 	mov rsp, rbp
; 	pop rbp

; 	ret
