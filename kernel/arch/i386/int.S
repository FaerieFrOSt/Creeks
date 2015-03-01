.extern isr_default_int, isr_clock_int, isr_kbd_int, isr_GP_exec, do_syscalls
.global _asm_default_int, _asm_irq_0, _asm_irq_1, _asm_exec_GP, _asm_syscalls

.include "arch/i386/regs.S"

_asm_default_int:
	SAVE_REGS
	call isr_default_int
	mov $0x20, %al
	out %al, $0x20
	RESTORE_REGS
	iret

_asm_irq_0:
	SAVE_REGS
	call isr_clock_int
	mov $0x20, %al
	out %al, $0x20
	RESTORE_REGS
	iret

_asm_irq_1:
	SAVE_REGS
	call isr_kbd_int
	mov $0x20, %al
	out %al, $0x20
	RESTORE_REGS
	iret

_asm_exec_GP:
	SAVE_REGS
	call isr_GP_exec
	RESTORE_REGS
	add $4, %esp
	iret

_asm_syscalls:
	SAVE_REGS
	push %eax
	call do_syscalls
	pop %eax
	RESTORE_REGS
	iret
