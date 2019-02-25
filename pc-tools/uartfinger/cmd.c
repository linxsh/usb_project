#include "cmd.h"
#include "uart.h"
#include <stdio.h>              // printf

#define CHECK_TAG(tag) do {        \
	if (tag != UART_FINGER_TAG_FLAG) \
		return -1;                    \
} while(0)

int uart_send_cmd(int fdcom, UART_FINGER_CMD *cmd)
{
	CHECK_TAG(cmd->tag);

	return port_send(fdcom, (char*)cmd, sizeof(UART_FINGER_CMD));
}

int uart_recv_cmd(int fdcom, UART_FINGER_CMD *cmd, int timeout_ms)
{
	unsigned int recv_len = 0;

	while (recv_len < sizeof(UART_FINGER_CMD)) {
		recv_len += port_recv(fdcom, (((char*)cmd) + recv_len),
				(sizeof(UART_FINGER_CMD) - recv_len), timeout_ms);
		if (recv_len == 0)
			return -1;
	}
	CHECK_TAG(cmd->tag);

	return recv_len;
}

int uart_send_data(int fdcom, unsigned char *buf, unsigned int len)
{
	return port_send(fdcom, (char*)buf, len);;
}

int uart_recv_data(int fdcom, unsigned char *buf, unsigned int len, int timeout_ms)
{
	unsigned int recv_len = 0;
	unsigned int start_transmit = 0;

	while (recv_len < len) {
		recv_len += port_recv(fdcom, ((char *)buf + recv_len),
				(len - recv_len), timeout_ms);
		if (recv_len == 0)
			return -1;
		else if (start_transmit == 0) {
			printf("start...\n");
			start_transmit = 1;
		}
	}

	return recv_len;
}
