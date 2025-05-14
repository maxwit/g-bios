#include <at91.h>

	.global udelay
udelay:
	ldr r1, =(ACLK_RATE >> 21)
	mul r1, r0, r1
0:
	subs r1, r1, #1
	bgt  0b
	bx  lr
