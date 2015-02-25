
#ifndef _GDT_H_
# define _GDT_H_

#include <stdint.h>

#define GDTBASE 0
#define GDTSIZE 0xff

struct	gdtdesc
{
	uint16_t	lim0_15;
	uint16_t	base0_15;
	uint8_t		base16_23;
	uint8_t		acces;
	uint8_t		lim16_19:4;
	uint8_t		other:4;
	uint8_t		base24_31;
} __attribute__ ((packed));

struct	gdtr
{
	uint16_t	limite;
	uint32_t	base;
} __attribute__ ((packed));

void	init_gdt(void);

#endif /* !_GDT_H_ */
