#include "kernel/gdt.h"
#include <string.h>

static struct gdtdesc	kgdt[GDTSIZE];
static struct gdtr		kgdtr;

static void	init_gdt_desc(uint32_t base, uint32_t limite, uint8_t acces, uint8_t other,
						struct gdtdesc *desc)
{
	desc->lim0_15 = limite & 0xffff;
	desc->base0_15 = base & 0xffff;
	desc->base16_23 = (base & 0xff0000) >> 16;
	desc->acces = acces;
	desc->lim16_19 = (limite & 0xf0000) >> 16;
	desc->other = other & 0xf;
	desc->base24_31 = (base & 0xff000000) >> 24;
}

void	init_gdt(void)
{
	init_gdt_desc(0, 0, 0, 0, &kgdt[0]);
	init_gdt_desc(0, 0xfffff, 0x9b, 0x0d, &kgdt[1]); //code
	init_gdt_desc(0, 0xfffff, 0x93, 0x0d, &kgdt[2]); //data
	init_gdt_desc(0, 0, 0x97, 0x0d, &kgdt[3]); // stack

	kgdtr.limite = GDTSIZE * 8;
	kgdtr.base = GDTBASE;

	memcpy((char*)kgdtr.base, (char*)kgdt, kgdtr.limite);

	asm("lgdtl (kgdtr)");

	asm("	movw $0x10, %ax		\n \
			movw %ax, %ds		\n \
			movw %ax, %es		\n \
			movw %ax, %fs		\n \
			movw %ax, %gs		\n \
			ljmp $0x08, $next	\n \
			next:				\n");
}
