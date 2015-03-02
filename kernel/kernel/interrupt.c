#include <stdint.h>
#include <stdio.h>
#include "kernel/tty.h"
#include "kernel/vga.h"
#include "kernel/io.h"
#include "kernel/kbd.h"
#include "kernel/gdt.h"

void	do_syscalls(int sys_num)
{
	uint16_t		ds_select;
	uint32_t		ds_base;
	struct gdtdesc	*ds;
	uint8_t			*msg;

	if (sys_num == 1)
	{
		asm("	mov 44(%%ebp), %%eax	\n \
				mov %%eax, %0			\n \
				mov 24(%%ebp), %%ax		\n \
				mov %%ax, %1" : "=m" (msg), "=m" (ds_select) :);
		ds = (struct gdtdesc *) (GDTBASE + (ds_select & 0xf8));
		ds_base = ds->base0_15 + (ds->base16_23 << 16) + (ds->base24_31 << 24);
		printf("%s", (char*)(ds_base + msg));
	} else
		printf("syscall %d\n", sys_num);
}

void	isr_GP_exec(void)
{
	terminal_error("GP fault\n");
	uint32_t	eip;
	uint16_t	cs;
	asm("	popw %0 \n \
			mov %%cs, %1" : "=m" (eip), "=m" (cs) :);
	printf("eip : %x\ncs : %x", (unsigned int)eip, cs);
	while (1);
}

void	isr_PF_exec(size_t addr)
{
	printf("PF fault at : %x\n", addr);
	while (1);
}

void	isr_default_int(void)
{
	terminal_debug("interrupt\n");
}

void	isr_clock_int(void)
{
	static int	tic = 0;
	static int	sec = 0;
	++tic;
	if (tic % 100 == 0)
	{
		++sec;
		tic = 0;
		printf(".");
	}
}

void	isr_kbd_int(void)
{
	uint8_t	i;
	static int	lshift_enable;
	static int	rshift_enable;
	static int	alt_enable;
	static int ctrl_enable;
	int	tmp = 0;

	do
	{
		i = inb(0x64);
	} while ((i & 0x01) == 0);
	i = inb(0x60);
	--i;
	if (i < 0x80)
		tmp = 1;
	else
	{
		tmp = 0;
		i -= 0x80;
	}
	switch (i)
	{
		case 0x29:
			lshift_enable = tmp;
			break;
		case 0x35:
			rshift_enable = tmp;
			break;
		case 0x1c:
			ctrl_enable = tmp;
			break;
		case 0x37:
			alt_enable = tmp;
			break;
		default:
			if (tmp)
				printf("%c", kbdmap[i * 4 + (lshift_enable || rshift_enable) + ctrl_enable + alt_enable]);
	}
}
