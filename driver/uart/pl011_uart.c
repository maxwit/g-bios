#include <io.h>
#include <init.h>
#include <uart/uart.h>

#define PL011_UART_BASE 0x09000000

#define UART_DR       0x00
#define UART_RSRECR   0x04
#define UART_FR       0x18
#define UART_ILPR     0x20
#define UART_IBRD     0x24
#define UART_FBRD     0x28
#define UART_LCR_H    0x2C
#define UART_CR       0x30

// #define UART_FR_BUSY  (1 << 3)  // UART is busy?
#define UART_FR_RXFE  (1 << 4)  // RX FIFO is empty
#define UART_FR_TXFF  (1 << 5)  // TX FIFO is full

#define UART_READB(reg) \
	readb(VA(PL011_UART_BASE + (reg)))

#define UART_WRITEB(reg, val) \
	writeb(VA(PL011_UART_BASE + (reg)), (val))
static __init int pl011_uart_init(void)
{
    return 0;
}

stdio_init(pl011_uart_init);

void uart_send_byte(__u8 b)
{
    UART_WRITEB(UART_DR, b);
}
__u8 uart_recv_byte()
{
    return UART_READB(UART_DR);
}

__u32 uart_rxbuf_count()
{
    return UART_READB(UART_FR) & UART_FR_RXFE;
}
