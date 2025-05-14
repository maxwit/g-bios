#pragma once

#include <omap4_register.h>

#define KHz(n) (1000 * (n))
#define MHz(n) (1000 * KHz(n))

#define OMAP4_SRAM_BASE 0x40200000
#define OMAP4_SRAM_SIZE 0x00004000

#define SDRAM_BASE  0x80000000
#define SDRAM_SIZE  0x04000000

#define ATAG_BASE   SDRAM_BASE

#define MPU_CLK    MHz(720)
#define L3_CLK     MHz(133)
#define L4_CLK     MHz(66.5)
#define HCLK_RATE  L3_CLK

#define INT_VECTOR_RAM_BASE 0x4020FFC8
