#include "kernel/idt.h"
#include "kernel/io.h"
#include <string.h>

void	_asm_default_int(void);
void	_asm_irq_0(void);
void	_asm_irq_1(void);

static struct idtdesc	kidt[IDTSIZE];
static struct idtr		kidtr;

static void	init_idt_desc(uint16_t select, uint32_t offset,
							uint16_t type, struct idtdesc *desc)
{
	desc->offset0_15 = offset & 0xffff;
	desc->select = select;
	desc->type = type;
	desc->offset16_31 = (offset & 0xffff0000) >> 16;
}

void	init_idt(void)
{
	int	i;

	//default system descriptor
	for (i = 0; i < IDTSIZE; ++i)
		init_idt_desc(0x08, (uint32_t) _asm_default_int, INTGATE, &kidt[i]);

	//clock
	init_idt_desc(0x08, (uint32_t) _asm_irq_0, INTGATE, &kidt[32]);
	//keyboard
	init_idt_desc(0x08, (uint32_t) _asm_irq_1, INTGATE, &kidt[33]);

	kidtr.limite = IDTSIZE * 8;
	kidtr.base = IDTBASE;

	memcpy((char*)kidtr.base, (char*)kidt, kidtr.limite);

	asm("lidtl (kidtr)");
}
