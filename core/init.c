/*
 *  comment here
 */

#include <stdio.h>
#include <delay.h>
#include <loader.h>
#include <init.h>
#include <uart/uart.h>

#ifdef CONFIG_DEFAULT_LOADER
extern struct loader_opt g_loader_begin[], g_loader_end[];

static inline void boot(struct loader_opt *loader)
{
	((void (*)())loader->load_addr)();
}

static int load_os(char key)
{
	int ret;
	struct loader_opt *loader;

	// case ignore
	if ('A' <= key && key <= 'Z')
		key += 'a' - 'A';

	for (loader = g_loader_begin; loader < g_loader_end; loader++) {
#ifdef CONFIG_VERBOSE
		printf("%c: 0x%x\n", loader->ckey[0], loader->prompt);
#endif

		if (loader->ckey[0] == key) {
#ifdef CONFIG_LOADER_MENU
			// FIXME: prompt for RAM/ROM
			printf("Loading from %s ...\n", loader->prompt);
#endif
			ret = loader->load(loader);
			if (ret < 0)
				return ret;

			printf("\n"); // FIXME: to be removed
			boot(loader);
		}

		// TODO: invalid key
	}

	return 0;
}
#endif

static const char __init_data banner[] = "\ng-bios " __GBIOS_VER__
#ifdef CONFIG_VERBOSE
	" (" __TIME__ ")"
#endif
	"\n";

static int __init show_banner(void)
{
	printf(banner);
	return 0;
}

stdio_post(show_banner);

#ifdef CONFIG_SDRAM_BASE
staic __init int ram_verify(void)
{
	unsigned long *p = VA(CONFIG_SDRAM_BASE);

	while (p < VA(CONFIG_SDRAM_BASE + CONFIG_SDRAM_SIZE)) {
		unsigned long val1 = readl(p), val2;

		val2 = ~val1;
		*p = val2;
		val1 = *p;

		if (val1 != val2) {
#ifdef CONFIG_VERBOSE
			printf("bad memory @ 0x%x\n", p);
#endif
			return 'M';
		}

		p++;
	}

	return 0;
}
#endif

int __init system_init(void)
{
	extern init_func_t init_call_begin[], init_call_end[];

	init_func_t *init_call = init_call_begin;
	while (init_call < init_call_end) {
		int ret = (*init_call)();
		if (ret < 0)
			return ret;
		init_call++;
	}

	return 0;
}

int main(void)
{
	read_cpu_id();

#ifdef CONFIG_DEFAULT_LOADER
	for (int i = 0; i < 0x100; i++) {
#else
	for (;;) {
#endif

#if 0
		char key;

		if (uart_rxbuf_count() > 0) {
			key = uart_recv_byte();

#ifdef CONFIG_LOADER_MENU
			do {
				char nkey;
				struct loader_opt *loader;
#endif

				load_os(key);

#ifdef CONFIG_LOADER_MENU
				printf("\nBoot Menu:\n");

				for (loader = g_loader_begin; loader < g_loader_end; loader++)
					printf("  [%c] %s\n", loader->ckey[0], loader->prompt);
				printf("  [q] Default\nEnter Your Choice\n");

				while (key == (nkey = uart_recv_byte()));

				key = nkey;
			} while ('q' != key);

			break;
#endif
		}
		udelay(0x1000);
#endif
	}

#ifdef CONFIG_DEFAULT_LOADER
	load_os(CONFIG_DEFAULT_LOADER);
#endif

	return 'm';
}
