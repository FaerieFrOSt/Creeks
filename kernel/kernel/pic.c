#include "kernel/io.h"

void	init_pic(void)
{
	//initialization ICW1
	outb(0x20, 0x11);
	outb(0xa0, 0x11);

	//ICW2
	outb(0x21, 0x20);
	outb(0xa1, 0x70);

	//ICW3
	outb(0x21, 0x04);
	outb(0xa1, 0x02);

	//ICW4
	outb(0x21, 0x01);
	outb(0xa1, 0x01);

	//interrupts masks
	outb(0x21, 0);
	outb(0xa1, 0);
}
