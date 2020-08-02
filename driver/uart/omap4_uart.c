#include <io.h>
#include <uart/uart.h>

#define UART_READB(reg) \
	readb(VA(UART_BASE + (reg)))

#define UART_WRITEB(reg, val) \
	writeb(VA(UART_BASE + (reg)), (val))

static void omap4_uart_send_byte(__u8 b)
{
	while (UART_READB(SSR_REG) & 0x1);
	UART_WRITEB(THR_REG, b);
}

static __u8 omap4_uart_recv_byte(void)
{
	while (!(UART_READB(LSR_REG) & 0x1));
	return UART_READB(RHR_REG);
}

__u32 uart_rxbuf_count()
{
	return UART_READB(LSR_REG) & 0x1;
}

static int omap4_uart_init(void)
{
	__u16 div = MHz(48) / 16 / 115200;

	// pad configure
	// TODO: support other uarts
	PAD_WRITEW(PADCONF_UART3_CTS_RCTX, PTU | IEN);
	PAD_WRITEW(PADCONF_UART3_RTS_SD, 0x0);
	PAD_WRITEW(PADCONF_UART3_RX_IRRX, IEN);
	PAD_WRITEW(PADCONF_UART3_TX_IRTX, 0x0);

	// uart init
	UART_WRITEB(IER_REG, 0x00);
	UART_WRITEB(MDR1_REG, 0x7);
	UART_WRITEB(LCR_REG, 0x83);
	UART_WRITEB(DLL_REG, div & 0xff);
	UART_WRITEB(DLH_REG, (div >> 8) & 0xff);
	UART_WRITEB(LCR_REG, 0x3);
	UART_WRITEB(MCR_REG, 0x3);
	UART_WRITEB(FCR_REG, 0x7);
	UART_WRITEB(MDR1_REG, 0x0);

	return 0;
}

DECLARE_UART_INIT(omap4_uart_init);
DECLARE_UART_SEND(omap4_uart_send_byte);
DECLARE_UART_RECV(omap4_uart_recv_byte);
