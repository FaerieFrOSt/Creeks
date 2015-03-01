.macro	SAVE_REGS
	pushal
	push %ds
	push %es
	push %fs
	push %gs
	push %ebx
	mov $0x10, %bx
	mov %bx, %ds
	pop %ebx
.endm

.macro	RESTORE_REGS
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popal
.endm
