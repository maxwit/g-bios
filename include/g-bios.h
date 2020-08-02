#pragma once

#include <autoconf.h>

#define GB_MAGIC  (('G' << 24) | ('b' << 16) | ('I' << 8) | 'o')

#ifndef __ASSEMBLY__
#include <types.h>

#define VA(x) ((void *)(x))

int soc_init(void);
int uart_init(void);
int sdram_init(void);

unsigned long read_cpu_id(void);

void hang(char err);
#endif
