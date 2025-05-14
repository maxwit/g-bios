#include <e310.h>

	.global udelay
udelay:
	li   t0, (HCLK_RATE >> 21)
	mul  t0, a0, t0

1:
	addi t0, t0, -1
	bgtz t0, 1b

	ret
