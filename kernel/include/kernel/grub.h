
#ifndef _GRUB_H_
# define _GRUB_H_

#include <stdint.h>

struct mb_partial_info
{
	unsigned long	flags;
	unsigned long	low_mem;
	unsigned long	high_mem;
	unsigned long	boot_device;
	unsigned long	cmdline;
};

#endif /* !_GRUB_H_ */
