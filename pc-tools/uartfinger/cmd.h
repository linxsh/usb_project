#ifndef __CMD_H__
#define __CMD_H__

#define UART_FINGER_TAG_FLAG 0x66696e67
typedef enum {
	FINGER_READ  = 0x1,
	FINGER_WRITE = 0x2,
	FINGER_ACK   = 0x3
} UART_FINGER_OPT_E;

typedef enum {
	FINGER_DATA  = 0x1,
	FINGER_CMD   = 0x2,
} UART_FINGER_FMT_E;

typedef struct {
	unsigned int      tag;
	UART_FINGER_OPT_E opt;
	UART_FINGER_FMT_E fmt;
	unsigned int      len; //即将发送数据长度
} UART_FINGER_CMD;


int uart_send_read_cmd (UART_FINGER_CMD *cmd);
int uart_send_write_cmd(UART_FINGER_CMD *cmd);
int uart_recv_ack_cmd  (UART_FINGER_CMD *cmd);

int uart_send_data(unsigned char *buf, unsigned int len);
int uart_recv_data(unsigned char *buf, unsigned int len);
#endif
