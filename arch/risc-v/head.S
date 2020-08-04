	.text
	.globl _start

_start:
	lw   sp, stack_base

	call system_init

	call main

hang:
	j hang

stack_base:
	.word __stack_base
