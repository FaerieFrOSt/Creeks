
#ifndef _PROCESS_H_
# define _PROCESS_H_

#include <stdint.h>
#include <stddef.h>

struct process
{
	size_t	pid;
	struct
	{
		uint32_t	eax, ecx, edx, ebx;
		uint32_t	esp, ebp, esi, edi;
		uint32_t	eip, eflags;
		uint32_t	cs:16, ss:16, ds:16, es:16, fs:16, gs:16;
		uint32_t	cr3;
	} regs __attribute__ ((packed));
} __attribute__ ((packed));

void	scheduler(void);

void	load_task(uint32_t *code_phys_addr, uint32_t *fn, size_t code_size);

#endif /* !_PROCESS_H_ */
