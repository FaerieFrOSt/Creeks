#include <stdint.h>
#include "kernel/gdt.h"
#include "kernel/process.h"
#include "kernel/tss.h"

struct process			p_list[32];
static struct process	*current = 0;
size_t					n_proc = 0;

void	do_switch(void);

extern struct tss	default_tss;

void	switch_to(int n, int mode)
{
	uint32_t	kesp, eflags;
	uint16_t	kss, ss, cs;

	current = &p_list[n];

	default_tss.ss0 = current->kstack.ss0;
	default_tss.esp0 = current->kstack.esp0;
	ss = current->regs.ss;
	cs = current->regs.cs;
	eflags = (current->regs.eflags | 0x200) & 0xffffbfff;

	if (mode == USERMODE)
	{
		kss = current->kstack.ss0;
		kesp = current->kstack.esp0;
	} else
	{
		kss = current->regs.ss;
		kesp = current->regs.esp;
	}

	asm ("	mov %0, %%ss	\n \
			mov %1, %%esp	\n \
			cmp %[KMODE], %[mode]	\n \
			je next			\n \
			push %2			\n \
			push %3			\n \
			next:			\n \
			push %4			\n \
			push %5			\n \
			push %6			\n \
			push %7			\n \
			ljmp $0x08, $do_switch" :: "m" (kss), "m" (kesp), "m" (ss), "m" (current->regs.esp),
			"m" (eflags), "m" (cs), "m" (current->regs.eip), "m" (current),
			[KMODE] "i" (KERNELMODE), [mode] "g" (mode));
}

void	scheduler(void)
{
	struct process	*p;
	uint32_t	*stack_ptr;

	asm("mov (%%ebp), %%eax; mov %%eax, %0" : "=m" (stack_ptr) : );

	if (!current && n_proc)
		switch_to(0, USERMODE);
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

		if (current->regs.cs != 0x08) //not during a system call
		{
			current->regs.esp = stack_ptr[17];
			current->regs.ss = stack_ptr[18];
		} else //int during a system call
		{
			current->regs.esp = stack_ptr[9] + 12;
			current->regs.ss = default_tss.ss0;
		}
		current->kstack.ss0 = default_tss.ss0;
		current->kstack.esp0 = default_tss.esp0;

		if (n_proc > current->pid + 1) //simple roundrobin
			p = &p_list[current->pid + 1];
		else
			p = &p_list[0];
		if (p->regs.cs != 0x08)
			switch_to(p->pid, USERMODE);
		else
			switch_to(p->pid, KERNELMODE);
	}
}
