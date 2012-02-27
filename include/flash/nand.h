#pragma once

#include <types.h>

// standard nand commands
#define NAND_CMMD_READ0         0x00
#define NAND_CMMD_READ1         0x01
#define NAND_CMMD_RNDOUT        0x05
#define NAND_CMMD_PAGEPROG      0x10
#define NAND_CMMD_READOOB       0x50
#define NAND_CMMD_ERASE1        0x60
#define NAND_CMMD_STATUS        0x70
#define NAND_CMMD_STATUS_MULTI  0x71
#define NAND_CMMD_SEQIN         0x80
#define NAND_CMMD_RNDIN         0x85
#define NAND_CMMD_READID        0x90
#define NAND_CMMD_ERASE2        0xd0
#define NAND_CMMD_RESET         0xff

#define NAND_CMMD_READSTART     0x30
#define NAND_CMMD_RNDOUTSTART   0xE0
#define NAND_CMMD_CACHEDPROG    0x15

#define NAND_CMMD_DEPLETE1      0x100
#define NAND_CMMD_DEPLETE2      0x38
#define NAND_CMMD_STATUS_MULTI  0x71
#define NAND_CMMD_STATUS_ERROR  0x72
#define NAND_CMMD_STATUS_ERROR0 0x73
#define NAND_CMMD_STATUS_ERROR1 0x74
#define NAND_CMMD_STATUS_ERROR2 0x75
#define NAND_CMMD_STATUS_ERROR3 0x76
#define NAND_CMMD_STATUS_RESET  0x7f
#define NAND_CMMD_STATUS_CLEAR  0xff

#define NAND_CMMD_NONE          -1

// Nand description
#define SOFT_ECC_DATA_LEN  256
#define SOFT_ECC_CODE_NUM  3

#define NAND_MAX_CHIPS        8
#define NAND_MAX_OOB_SIZE    64
#define NAND_MAX_PAGESIZE    2048

#define NAND_NCE        0x01
#define NAND_CLE        0x02
#define NAND_ALE        0x04

#define NAND_CTRL_CLE        (NAND_NCE | NAND_CLE)
#define NAND_CTRL_ALE        (NAND_NCE | NAND_ALE)
#define NAND_CTRL_CHANGE    0x80

#define NAND_STATUS_FAIL        0x01
#define NAND_STATUS_FAIL_N1     0x02
#define NAND_STATUS_TRUE_READY  0x20
#define NAND_STATUS_READY       0x40
#define NAND_STATUS_WP          0x80

#define NAND_ECC_READ        0
#define NAND_ECC_WRITE        1
#define NAND_ECC_READSYN    2

#define NAND_NO_AUTOINCR    0x00000001
#define NAND_BUSWIDTH_16    0x00000002
#define NAND_NO_PADDING        0x00000004
#define NAND_CACHEPRG        0x00000008
#define NAND_COPYBACK        0x00000010
#define NAND_IS_AND            0x00000020
#define NAND_4PAGE_ARRAY    0x00000040
#define BBT_AUTO_REFRESH    0x00000080
#define NAND_NO_READRDY        0x00000100
#define NAND_NO_SUBPAGE_WRITE    0x00000200

#define NAND_SAMSUNG_LP_OPTIONS \
	(NAND_NO_PADDING | NAND_CACHEPRG | NAND_COPYBACK)

#define NAND_CANAUTOINCR(nand) (!(nand->flags & NAND_NO_AUTOINCR))
#define NAND_MUST_PAD(nand) (!(nand->flags & NAND_NO_PADDING))
#define NAND_HAS_CACHEPROG(nand) ((nand->flags & NAND_CACHEPRG))
#define NAND_HAS_COPYBACK(nand) ((nand->flags & NAND_COPYBACK))

#define NAND_CHIP_OPTIONS_MSK    (0x0000ffff & ~NAND_NO_AUTOINCR)
#define NAND_USE_FLASH_BBT    0x00010000
#define NAND_SKIP_BBTSCAN    0x00020000
#define NAND_OWN_BUFFERS    0x00040000

#define LP_OPTIONS (NAND_SAMSUNG_LP_OPTIONS | NAND_NO_READRDY | NAND_NO_AUTOINCR)
#define LP_OPTIONS16 (LP_OPTIONS | NAND_BUSWIDTH_16)

struct nand_desc {
	int   id;
	__u32 size;
	__u32 flags;
};

#define NAND_CHIP_DESC(n, i, w, c, e, f) \
	{.id = i, .size = (c) << 20 | (w), .flags = f}

struct nand_chip;

int nand_init(struct nand_chip *);
int nand_probe(struct nand_chip *);
void *nand_read_page(struct nand_chip *, __u32, void *);

struct nand_chip {
	void *cmmd_port;
	void *addr_port;
	void *data_port;

	size_t write_size;
	size_t chip_size;

	int (*nand_ready)(struct nand_chip *);
	void *(*read_buff)(struct nand_chip *, void *, size_t);
};
