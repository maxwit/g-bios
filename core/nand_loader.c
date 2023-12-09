#include <io.h>
#include <stdio.h>
#include <loader.h>
#include <flash/nand.h>

// FIXME
#ifndef CONFIG_OS_LOAD_SIZE
#define CONFIG_OS_LOAD_SIZE KB(512)
#endif

static int nand_do_load(struct nand_chip *nand, __u32 nstart, void *mstart)
{
	__u32 curr_page, last_page;
	__u32 wshift;
	__u32 load_size;
	void *buff;

	for (wshift = 0; wshift < WORD_BITS; wshift++) {
		if ((1 << wshift) == nand->write_size)
			break;
	}

	if (WORD_BITS == wshift)
		return -1;

	curr_page = nstart >> wshift;

	buff = nand_read_page(nand, curr_page, mstart);

	if (WITOS_MAGIC == readl(VA(mstart + WITOS_MAGIC_OFFSET))) {
		load_size = readl(VA(mstart + WITOS_SIZE_OFFSET));
#ifdef CONFIG_VERBOSE
		printf("WitOS detected.\n");
#endif
	} else {
		load_size = CONFIG_OS_LOAD_SIZE;
	}

	last_page = curr_page + ((load_size - 1) >> wshift);

#ifdef CONFIG_VERBOSE
	printf("Nand loader: memory = 0x%x, nand = 0x%x, size = 0x%x\n", mstart,
		   nstart, load_size);
#endif

	while (++curr_page <= last_page)
		buff = nand_read_page(nand, curr_page, buff);

	return buff - mstart;
}

static int nand_loader(struct loader_opt *opt)
{
	int ret;
	struct nand_chip nand = { 0 }; // nand_chip must be initialized

	ret = nand_init(&nand);
	if (ret < 0)
		return ret;

	ret = nand_probe(&nand);
	if (ret < 0)
		return ret;

	ret = nand_do_load(&nand, CONFIG_OS_NAND_START, opt->load_addr);

	return ret;
}

REGISTER_LOADER(n, nand_loader, "NAND");
