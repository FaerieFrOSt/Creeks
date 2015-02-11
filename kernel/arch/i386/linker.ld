ENTRY(_start) /* entry point */

SECTIONS
{
	. = 1M; /*begin at 1M in memory */
	.text BLOCK(4K) : ALIGN(4K) /*so GRUB can recognize the kernel */
	{
		*(.multiboot)
		*(.text)
	}
	.rodata BLOCK(4K) : ALIGN(4K) /* read only data */
	{
		*(.rodata)
	}
	.data BLOCK(4K) : ALIGN(4K) /* read write data */
	{
		*(.data)
	}
	.bss BLOCK(4K) : ALIGN(4K) /* stack, uninitialized data */
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
}
