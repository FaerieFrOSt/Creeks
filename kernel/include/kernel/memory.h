
#ifndef _MEMORY_H_
# define _MEMORY_H_

#define PAGING_FLAG 0x80000000 //CR => bit 31

#define PD0_ADDR 0x20000 //addr page directory kernel
#define PT0_ADDR 0x21000 //addr page table[0] kernel

void	init_mm(void);

#endif /* !_MEMORY_H_ */
