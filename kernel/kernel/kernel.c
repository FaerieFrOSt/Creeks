#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/vga.h>
#include <kernel/io.h>
#include <kernel/pic.h>
#include <kernel/tss.h>

extern struct tss	default_tss;

void	task1(void)
{
	while(1);
}

void	kernel_early(void)
{
	terminal_initialize();

	terminal_debug("loading idt...");
	init_idt();
	terminal_ok();
	terminal_debug("loading pic...");
	init_pic();
	terminal_ok();
	terminal_debug("loading new gdt...");
	init_gdt();
	terminal_ok();
	terminal_debug("loading new tss...");
	asm("	movw $0x38, %ax	\n \
			ltr %ax"); //0x38 => &kgdt[7]
	terminal_ok();
	asm("	movw $0x18, %ax	\n \
			movw %ax, %ss	\n \
			movl $0x20000, %esp");
}

void	kernel_main(void)
{
	terminal_debug("allowing interrupts...");
	sti;
	terminal_ok();
	terminal_debug("Switching to user mode ring 3\n");
	memcpy((char*)0x30000, &task1, 100);
	asm("	cli			\n \
			push $0x33	\n \
			push $0x30000	\n \
			pushfl		\n \
			popl %%eax	\n \
			orl $0x200, %%eax	\n \
			and $0xffffbfff, %%eax	\n \
			push %%eax	\n \
			push $0x23	\n \
			push $0x0	\n \
			movl $0x20000, %0	\n \
			movw $0x2B, %%ax	\n \
			movw %%ax, %%ds	\n \
			iret" : "=m" (default_tss.esp0) :);
	terminal_error("Critical error! Halting system");
	asm("hlt");
	while (1);
}
