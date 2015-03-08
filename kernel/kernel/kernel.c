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
#include <kernel/memory.h>
#include <kernel/process.h>
#include <kernel/grub.h>

extern struct tss	default_tss;

void	kernel_main(void);

void	task1(void)
{
	char	*msg = (char*)0x40001000;
	int		i;

	msg[0] = 't';
	msg[1] = 'a';
	msg[2] = 's';
	msg[3] = 'k';
	msg[4] = '1';
	msg[5] = '\n';
	msg[6] = 0;
	while(1)
	{
		asm("mov %0, %%ebx; mov $0x01, %%eax; int $0x30" :: "m" (msg));
		for (i = 0; i < 1000000; ++i);
	}
}

void	task2(void)
{
	char	*msg = (char*)0x40001000;
	int		i;

	msg[0] = 't';
	msg[1] = 'a';
	msg[2] = 's';
	msg[3] = 'k';
	msg[4] = '2';
	msg[5] = '\n';
	msg[6] = 0;
	while(1)
	{
		asm("mov %0, %%ebx; mov $0x01, %%eax; int $0x30" :: "m" (msg));
		for (i = 0; i < 1000000; ++i);
	}
}

void	kernel_early(struct mb_partial_info *mbi)
{
	terminal_initialize();

	terminal_debug("loading kernel...\n");
	terminal_debug("RAM detected : ");
	printf("%luk (lower), %luk (upper). Total : %luM\n", mbi->low_mem, mbi->high_mem,
			(mbi->high_mem - mbi->low_mem) / 1024);
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
	terminal_debug("activating paging...");
	init_mm();
	terminal_ok();
	asm("	movw $0x18, %ax	\n \
			movw %ax, %ss	\n \
			movl $0x120000, %esp");
	kernel_main();
}

void	kernel_main(void)
{
	terminal_debug("creating task1...");
	/* load_task((uint32_t*)0x200000, (uint32_t*)&task1, 0x2000); */
	/* load_task((uint32_t*)0x210000, (uint32_t*)&task2, 0x2000); */
	terminal_ok();
	terminal_debug("allowing interrupts...");
	sti;
	terminal_ok();
	while (1);
}
