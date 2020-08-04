#pragma once

#include <types.h>

void __io_putchar(char ch);

int putchar(int);

char *gets(char *);

int puts(const char *);

int printf(const char *, ...);

int sprintf(char *, const char *, ...);

int snprintf(char *, size_t, const char *, ...);

int fflush(int);

// right here?
#ifdef CONFIG_VERBOSE
#define DPRINT(fmt, args ...) \
	printf(fmt, ##args)
#define GEN_DBG(fmt, args ...) \
	printf("%s() line %d: " fmt, __func__, __LINE__, ##args)
#else
#define DPRINT(fmt, args ...)
#define GEN_DBG(fmt, args ...)
#endif
