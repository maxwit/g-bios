	.text
	.globl _start

_start:
	li a0, 0x10012000

	lw a1, 8(a0)
	ori a1, a1, 1 << 5
	sw a1, 8(a0)

	lw a1, 0xc(a0)
	ori a1, a1, 1 << 5
	sw a1, 0xc(a0)

	li sp, 0x80010000
	call main
	j hang

hang:
	j hang
