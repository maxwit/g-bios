/*
 *  comment here
 *
 */

	.text
	.globl _start
	.globl hang

#if defined(CONFIG_OMAP3) || defined(CONFIG_OMAP4)
	.word (_end - _start)
	.word _start
#endif
_start:
	b     handle_reset
	ldr   pc, [pc, #20]
	ldr   pc, [pc, #20]
	ldr   pc, [pc, #20]
	ldr   pc, [pc, #20]
	.word (_end - _start)   @ for AT91SAM926X
	ldr   pc, [pc, #20]
	ldr   pc, [pc, #20]

	.word handle_undef
	.word handle_swi
	.word handle_iabt
	.word handle_dabt
	.word GB_MAGIC
	.word handle_irq
	.word handle_fiq

handle_reset:
	msr  cpsr, #(ARM_MODE_SVC | ARM_INT_MASK)

	@ init default sp
	ldr sp, =__stack_base
	@ TODO: verify the stack memory

#ifdef CONFIG_OMAP3
	mov r12, #0x1		@ to invalidate L2
	.word 0xE1600070	@ Call SMI monitor
#endif

	bl   system_init
	cmp  r0, #0
	bne  hang

	bl   main
	b    atexit

#if 1
handle_undef:
	mov  r0, #'U'
	b    hang

handle_swi:
	mov  r0, #'S'
	b    hang

handle_iabt:
	mov  r0, #'i'
	b    hang

handle_dabt:
	mov  r0, #'d'
	b    hang

handle_irq:
	mov  r0, #'I'
	b    hang

handle_fiq:
	mov  r0, #'F'

#else
handle_undef:
handle_swi:
handle_iabt:
handle_dabt:
handle_irq:
handle_fiq:
	mov  r0, #'E'
#endif

atexit:
	rsb  r0, r0, #'0'
	b    hang
hang:
	bl   __io_putchar
	sub  pc, pc, #8

	.align 2
