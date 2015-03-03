.set ALIGN, 1 << 0				# align loaded modules on page boundaries
.set MEMINFO, 1 << 1			# provide memory map
.set FLAGS, ALIGN | MEMINFO		# multiboot flag field
.set MAGIC, 0x1BADB002			# magic number so GRUB can find the kernel
.set CHECKSUM, -(MAGIC + FLAGS)	# prove it's multiboot

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .text
.global _start
.type _start, @function
_start:
	cli
	call _init
	call kernel_early
	cli
	hlt

.Lhang:
	jmp .Lhang
