#include <stdint.h>
#include "kernel/gdt.h"
#include "kernel/process.h"
#include "kernel/tss.h"

struct process			p_list[32];
static struct process	*current = 0;
size_t					n_proc = 0;

void	do_switch(void);

extern struct tss	default_tss;

void	scheduler(void)
{
	uint32_t	*stack_ptr;
	uint32_t	esp0, eflags;
	uint16_t	ss, cs;

	asm("mov (%%ebp), %%eax; mov %%eax, %0" : "=m" (stack_ptr) : );

	if (!current && n_proc)
		current = &p_list[0];
	else if (n_proc <= 1)
		return;
	else if (n_proc > 1)
	{
		current->regs.eflags = stack_ptr[16];
		current->regs.cs = stack_ptr[15];
		current->regs.eip = stack_ptr[14];
		current->regs.eax = stack_ptr[13];
		current->regs.ecx = stack_ptr[12];
		current->regs.edx = stack_ptr[11];
		current->regs.ebx = stack_ptr[10];
		current->regs.ebp = stack_ptr[8];
		current->regs.esi = stack_ptr[7];
		current->regs.edi = stack_ptr[6];
		current->regs.ds = stack_ptr[5];
		current->regs.es = stack_ptr[4];
		current->regs.fs = stack_ptr[3];
		current->regs.gs = stack_ptr[2];
		current->regs.esp = stack_ptr[17];
		current->regs.ss = stack_ptr[18];

		default_tss.esp0 = (uint32_t) (stack_ptr + 19);
		if (n_proc > current->pid + 1) //simple roundrobin
			current = &p_list[current->pid + 1];
		else
			current = &p_list[0];
	}

	ss = current->regs.ss;
	cs = current->regs.cs;
	eflags = (current->regs.eflags | 0x200) & 0xffffbfff;
	esp0 = default_tss.esp0;
	asm("	mov %0, %%esp		\n \
			push %1				\n \
			push %2				\n \
			push %3				\n \
			push %4				\n \
			push %5				\n \
			push %6				\n \
			ljmp $0x08, $do_switch" :: "m" (esp0), "m" (ss), "m" (current->regs.esp), \
			"m" (eflags), "m" (cs), "m" (current->regs.eip), "m" (current));
}
