#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/vga.h>

void	kernel_early(void)
{
	terminal_initialize();

	terminal_setcolor(make_color(COLOR_CYAN, COLOR_BLACK));
	printf("KERNEL");
	terminal_removecolor();
	printf(" : loading new gdt...\t");
	init_gdt();
	asm("	movw $0x18, %ax	\n \
			movw %ax, %ss	\n \
			movl $0x20000, %esp");
}

void	kernel_main(void)
{
	terminal_setcolor(make_color(COLOR_WHITE, COLOR_BLACK));
	printf("...DONE!");
	terminal_removecolor();
	printf("\n");
	printf("Hello world!\n");
	printf("%x\n", 10);
	while (1);
}
