#pragma once

#include <types.h>

static inline __u8 readb(void *mem)
{
	return *(volatile __u8 *)mem;
}

static inline void writeb(void *mem, __u8 val)
{
	*(volatile __u8 *)mem = val;
}

static inline __u16 readw(void *mem)
{
	return *(volatile __u16 *)mem;
}

static inline void writew(void *mem, __u16 val)
{
	*(volatile __u16 *)mem = val;
}

static inline __u32 readl(void *mem)
{
	return *(volatile __u32 *)mem;
}

static inline void writel(void *mem, __u32 val)
{
	*(volatile __u32 *)mem = val;
}

static inline void bits_writel(void *mem, __u32 mask, __u32 val)
{
	__u32 tmp;

	tmp = readl(VA(mem));
	tmp &= ~mask;
	tmp |= val & mask;
	writel(VA(mem), tmp);
}
