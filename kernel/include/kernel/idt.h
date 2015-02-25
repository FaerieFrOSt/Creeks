
#ifndef _IDT_H_
# define _IDT_H_

#include <stdint.h>

#define IDTBASE 0x800
#define IDTSIZE 0xff

#define INTGATE 0x8e00

struct	idtdesc
{
	uint16_t	offset0_15;
	uint16_t	select;
	uint16_t	type;
	uint16_t	offset16_31;
} __attribute__ ((packed));

struct	idtr
{
	uint16_t	limite;
	uint32_t	base;
} __attribute__ ((packed));

void	init_idt(void);

#endif /* !_IDT_H_ */
