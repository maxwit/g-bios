#pragma once

#include <types.h>
#include <init.h>

// to be removed
#define LM_NAND      'n'
#define LM_KERMIT    'k'
#define LM_YMODEM    'y'
#define LM_RAM       'r'
#define LM_MMC       'm'

#define __GBIOS_LOADER__    __attribute__((section(".witrom_loader")))

struct loader_opt {
	void *load_addr; // FIXME: void *load_addr[2];
	int  load_flash; // FIXME
	int  load_size;
#ifdef CONFIG_LOADER_MENU
	const char *prompt;
#endif
	char ckey[2];
	int (*main)(struct loader_opt *opt);
};

// DO NOT add "static" here
#ifdef CONFIG_LOADER_MENU
#define REGISTER_LOADER(key, routine, desc) \
	const __USED__ __GBIOS_LOADER__ struct loader_opt __witrom_loader_##key = \
		{.ckey = #key, .main = routine, .prompt = desc}
#else
#define REGISTER_LOADER(key, routine, desc) \
	const __USED__ __GBIOS_LOADER__ struct loader_opt __witrom_loader_##key = \
		{.ckey = #key, .main = routine}
#endif
