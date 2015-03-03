
#ifndef _MEMORY_H_
# define _MEMORY_H_

#include <stdint.h>
#include <stddef.h>

#define PAGESIZE 4096
#define RAM_MAXPAGE 0x10000

#define VADDR_PD_OFFSET(addr)	(((addr) & 0xffc00000) >> 22)
#define VADDR_PT_OFFSET(addr)	(((addr) & 0x003ff000) >> 12)
#define VADDR_PG_OFFSET(addr)	((addr) & 0x00000fff)
#define PAGE(addr)				((uint32_t)(addr) >> 12)

#define PAGING_FLAG 0x80000000 //CR => bit 31

#define USER_OFFSET 0x40000000
#define USER_STACK 0xe0000000

#define set_page_frame_used(page)	mem_bitmap[((uint32_t)(page)) / 8] |= (1 << (((uint32_t)(page)) % 8))
#define release_page_frame(p_addr)	mem_bitmap[((uint32_t)(p_addr) / PAGESIZE) / 8] &= ~(1 << (((uint32_t)(p_addr) / PAGESIZE) % 8))

struct pd_entry
{
	uint32_t	present:1;
	uint32_t	writable:1;
	uint32_t	user:1;
	uint32_t	pwt:1;
	uint32_t	pcd:1;
	uint32_t	accessed:1;
	uint32_t	_unused:1;
	uint32_t	page_size:1;
	uint32_t	global:1;
	uint32_t	avail:3;
	uint32_t	page_table_base:20;
} __attribute__ ((packed));

struct pt_entry
{
	uint32_t	present:1;
	uint32_t	writable:1;
	uint32_t	user:1;
	uint32_t	pwt:1;
	uint32_t	pcd:1;
	uint32_t	accessed:1;
	uint32_t	dirty:1;
	uint32_t	pat:1;
	uint32_t	global:1;
	uint32_t	avail:3;
	uint32_t	page_base:20;
} __attribute__ ((packed));

void		init_mm(void);

char		*get_page_frame(void);

uint32_t	*pd_create(uint32_t *code_phys_addr, size_t code_size);

#endif /* !_MEMORY_H_ */
