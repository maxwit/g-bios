#pragma once

#include <autoconf.h>

#define MKFOURCC(a, b, c, d)    (((a) << 24) | (b) << 16 | ((c) << 8) | (d))

#ifndef __ASSEMBLY__
#include <types.h>

#define VA(x) ((void *)(x))

int cpu_init(void);
int soc_init(void);
int uart_init(void);
int mem_init(void);

unsigned long read_cpu_id(void);

void hang(char err);
#endif
