.global do_switch

do_switch:
	mov (%esp), %esi
	pop %eax	#to get *current

	#prepare registers
	pushl 4(%esi)
	pushl 8(%esi)
	pushl 12(%esi)
	pushl 16(%esi)
	pushl 24(%esi)
	pushl 28(%esi)
	pushl 32(%esi)
	pushl 48(%esi)
	pushl 50(%esi)
	pushl 52(%esi)
	pushl 54(%esi)

	#remove PIC mask
	mov $0x20, %al
	out %al, $0x20

	#load page tables
	mov 56(%esi), %eax
	mov %eax, %cr3

	#load registers
	pop %gs
	pop %fs
	pop %es
	pop %ds
	pop %edi
	pop %esi
	pop %ebp
	pop %ebx
	pop %edx
	pop %ecx
	pop %eax

	iret
