#include <io.h>
#include <delay.h>
#include <init.h>
#include <uart/uart.h>

static void e310_uart_send_byte(__u8 b)
{

}

static __u8 e310_uart_recv_byte(void)
{
	return 0;
}

__u32 uart_rxbuf_count()
{
	return 0;
}

static int e310_uart_init(void)
{
	return 0;
}

stdio_init(e310_uart_init);

DECLARE_UART_SEND(e310_uart_send_byte);
DECLARE_UART_RECV(e310_uart_recv_byte);
