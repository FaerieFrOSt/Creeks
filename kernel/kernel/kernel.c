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
	asm("	movw $0x18, %ax	\n \
			movw %ax, %ss	\n \
			movl $0x20000, %esp");
}

void	kernel_main(void)
{
	terminal_ok();
	terminal_debug("allowing interrupts...");
	sti;
	terminal_ok();
	while (1);
}
