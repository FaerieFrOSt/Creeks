#include <stdint.h>
#include "kernel/memory.h"

void	init_mm(void)
{
	uint32_t	*pd0;
	uint32_t	*pt0;
	uint32_t	page_addr;
	uint32_t	i;

	//page directory creation
	pd0 = (uint32_t*)PD0_ADDR;
	pd0[0] = PT0_ADDR;
	pd0[0] |= 3; //3 => in memory + R/W
	for (i = 1; i < 1024; ++i)
		pd0[i] = 0;

	//page table[0] creation
	pt0 = (uint32_t*)PT0_ADDR;
	page_addr = 0;
	for (i = 0; i < 1024; ++i)
	{
		pt0[i] = page_addr;
		pt0[i] |= 3;
		page_addr += 4096; //4Ko/page
	}

	asm("	mov %0, %%eax		\n \
			mov %%eax, %%cr3	\n \
			mov %%cr0, %%eax	\n \
			or %1, %%eax		\n \
			mov %%eax, %%cr0" :: "i" (PD0_ADDR), "i" (PAGING_FLAG));
}
