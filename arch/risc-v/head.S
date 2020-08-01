	.text
	.global _start

_start:
	li a0, 11
	li a1, 22
	add a2, a0, a1
	j hang

hang:
	j hang
