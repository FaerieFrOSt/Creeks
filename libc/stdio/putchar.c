#include <stdio.h>

#if defined(__is_creeks_kernel)
#include <kernel/tty.h>
#endif

int	putchar(int ic)
{
#if defined(__is_creeks_kernel)
	char	c = (char)ic;
	terminal_write(&c, sizeof(c));
#else
	// TODO: write a system call for putchar
#endif
	return ic;
}
