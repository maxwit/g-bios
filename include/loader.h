#pragma once

// to be removed
#define LM_NAND      'n'
#define LM_KERMIT    'k'
#define LM_YMODEM    'y'
#define LM_RAM       'r'
#define LM_MMC       'm'

#define __GBIOS_LOADER__    __attribute__((section(".os_loader")))

struct loader_opt {
	void *load_addr;
	int  load_size;
#ifdef CONFIG_LOADER_MENU
	const char *prompt;
#endif
	char ckey[2];
	int (*load)(struct loader_opt *opt);
};

// DO NOT add "static" here
#ifdef CONFIG_LOADER_MENU
#define REGISTER_LOADER(key, routine, desc) \
	const __USED__ __GBIOS_LOADER__ struct loader_opt __os_loader_##key = \
		{.load_addr = (void *)CONFIG_OS_RAM_ADDR, .ckey = #key, .load = routine, .prompt = desc}
#else
#define REGISTER_LOADER(key, routine, desc) \
	const __USED__ __GBIOS_LOADER__ struct loader_opt __os_loader_##key = \
		{.load_addr = (void *)CONFIG_OS_RAM_ADDR, .ckey = #key, .load = routine}
#endif

#define WITOS_MAGIC          (('w' << 24) | ('i' << 16) | ('O' << 8) | 'S')
#define WITOS_MAGIC_OFFSET   32
#define WITOS_SIZE_OFFSET    20
