#include <stdint.h>
#include <stdio.h>
#include "kernel/tty.h"
#include "kernel/vga.h"
#include "kernel/io.h"
#include "kernel/kbd.h"

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
		/* printf("clock\n"); */
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
