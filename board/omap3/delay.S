#include <omap3.h>

	.global udelay
udelay:
	ldr r1, =(MPU_CLK >> 21)
	mul r1, r0, r1
0:
	subs r1, r1, #1
	bgt  0b
	bx  lr
