#pragma once

#define PADCONF_UART3_CTS_RCTX		0x0140
#define PADCONF_UART3_RTS_SD		0x0142
#define PADCONF_UART3_RX_IRRX		0x0144
#define PADCONF_UART3_TX_IRTX		0x0146

#define IEN             (1 << 8)
#define IDIS            (0 << 8)
#define PTU             (3 << 3)
#define PTD             (1 << 3)
#define EN              (1 << 3)
#define DIS             (0 << 3)

#define PAD_WRITEW(reg, val) \
	writew(VA(OMAP44XX_CTRL_PADCONF_CORE_BASE + (reg)), (val))
