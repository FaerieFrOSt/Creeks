#include <stdint.h>
#include "kernel/memory.h"

static uint32_t	*pd0; //kernel page directory
static uint32_t	*pt0; //kernel page table
uint8_t			mem_bitmap[RAM_MAXPAGE / 8]; //bitmap page allocation (1Go)

char	*get_page_frame(void)
{
	int	byte, bit;
	int	page = -1;

	for (byte = 0; byte < RAM_MAXPAGE / 8; ++byte)
		if (mem_bitmap[byte] != 0xff)
			for (bit = 0; bit < 8; ++bit)
				if (!(mem_bitmap[byte] & (1 << bit)))
				{
					page = 8 * byte + bit;
					set_page_frame_used(page);
					return (char*)(page * PAGESIZE);
				}
	return (char*)-1;
}

void	init_mm(void)
{
	uint32_t	page_addr;
	uint32_t	i, pg;

	for (pg = 0; pg < RAM_MAXPAGE / 8; ++pg)
		mem_bitmap[pg] = 0;

	//reserved pages for kernel
	for (pg = PAGE(0x0); pg < PAGE(0x1000); ++pg) //idt, gdt and other stuff are here
		set_page_frame_used(pg);
	for (pg = PAGE(0x100000); pg < PAGE(0x120000); ++pg)
		set_page_frame_used(pg);

	//reserved pages for hardware
	for (pg = PAGE(0xA0000); pg < PAGE(0x100000); ++pg)
		set_page_frame_used(pg);

	//allocating one page for page directory and one for page table[0]
	pd0 = (uint32_t*)get_page_frame();
	pt0 = (uint32_t*)get_page_frame();

	//page directory creation
	pd0[0] = (uint32_t)pt0;
	pd0[0] |= 3; //3 => in memory + R/W
	for (i = 1; i < 1024; ++i)
		pd0[i] = 0;

	//page table[0] creation
	page_addr = 0;
	for (i = 0; i < 1024; ++i)
	{
		pt0[i] = page_addr;
		pt0[i] |= 3; //3 => in memory + R/W
		page_addr += 4096; //4Ko/page
	}

	asm("	mov %0, %%eax		\n \
			mov %%eax, %%cr3	\n \
			mov %%cr0, %%eax	\n \
			or %1, %%eax		\n \
			mov %%eax, %%cr0" :: "m" (pd0), "i" (PAGING_FLAG));
}

uint32_t	*pd_create(uint32_t *code_phys_addr, size_t code_size)
{
	uint32_t	*pd, *pt;
	uint32_t	i, j;
	uint32_t	pages;

	//page directory
	pd = (uint32_t*)get_page_frame();
	for (i = 1; i < 1024; ++i)
		pd[i] = 0;
	//kernel space
	pd[0] = pd0[0];
	pd[0] |= 3;
	//user space
	if (code_size % PAGESIZE)
		pages = code_size / PAGESIZE + 1;
	else
		pages = code_size / PAGESIZE;
	for (i = 0; pages; ++i)
	{
		pt = (uint32_t*) get_page_frame();
		pd[(USER_OFFSET + i * PAGESIZE * 1024) >> 22] = (uint32_t)pt;
		pd[(USER_OFFSET + i * PAGESIZE * 1024) >> 22] |= 7;
		for (j = 0; j < 1024 && pages; ++j, --pages)
		{
			pt[j] = (uint32_t)(code_phys_addr + i * PAGESIZE * 1024 + j * PAGESIZE);
			pt[j] |= 7;
		}
	}
	return pd;
}
