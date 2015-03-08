#include <string.h>
#include "kernel/process.h"
#include "kernel/memory.h"
#include "kernel/tty.h"

extern uint8_t			mem_bitmap[RAM_MAXPAGE / 8];
extern struct process	p_list[32];
extern size_t			n_proc;

void	load_task(uint32_t *code_phys_addr, uint32_t *fn, size_t code_size)
{
	uint32_t	page_base, pages, kstack_base;
	uint32_t	*pd;
	uint32_t	i;

	memcpy((char*)code_phys_addr, (char*)fn, code_size);
	page_base = (uint32_t)PAGE(code_phys_addr);
	if (code_size % PAGESIZE)
		pages = code_size / PAGESIZE + 1;
	else
		pages = code_size / PAGESIZE;

	for (i = 0; i < pages; ++i)
		set_page_frame_used(page_base + i);

	pd = pd_create(code_phys_addr, code_size);

	kstack_base = (uint32_t)get_page_frame();
	if (kstack_base > 0x400000)
	{
		terminal_error("not enough memory to create a kernel stack\n");
		return;
	}
	p_list[n_proc].pid = n_proc;
	p_list[n_proc].regs.ss = 0x33;
	p_list[n_proc].regs.esp = 0x40001000;
	p_list[n_proc].regs.cs = 0x23;
	p_list[n_proc].regs.eip = 0x40000000;
	p_list[n_proc].regs.ds = 0x2b;
	p_list[n_proc].regs.es = 0x2b;
	p_list[n_proc].regs.fs = 0x2b;
	p_list[n_proc].regs.gs = 0x2b;
	p_list[n_proc].regs.cr3 = (uint32_t)pd;

	p_list[n_proc].kstack.ss0 = 0x18;
	p_list[n_proc].kstack.esp0 = kstack_base + PAGESIZE;

	p_list[n_proc].regs.eax = 0;
	p_list[n_proc].regs.ecx = 0;
	p_list[n_proc].regs.edx = 0;
	p_list[n_proc].regs.ebx = 0;

	p_list[n_proc].regs.ebp = 0;
	p_list[n_proc].regs.esi = 0;
	p_list[n_proc].regs.edi = 0;

	++n_proc;
}

