#include <io.h>
#include <init.h>
#include <uart/uart.h>

#define PL011_UART_BASE 0x09000000

static __init int pl011_uart_init(void)
{
    return 0;
}

void __io_putchar(__u8 b)
{
    // TODO: check FIFO
    writeb(VA(PL011_UART_BASE), b);
}

stdio_init(pl011_uart_init);

void uart_send_byte(__u8 b)
{
    writeb(VA(PL011_UART_BASE), b);
}
__u8 uart_recv_byte()
{
    return readb(VA(PL011_UART_BASE));
}

__u32 uart_rxbuf_count()
{
    return 0;
}
