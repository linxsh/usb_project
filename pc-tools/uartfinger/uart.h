#ifndef __UART_H__
#define __UART_H__

//串口结构
typedef struct{
	char  *tty;      //tty0, ttyUSB0, ...
	char  prompt;   //prompt after reciving data
	int   baudrate; //baudrate
	char  databit;  //data bits, 5, 6, 7, 8
	char  debug;    //debug mode, 0: none, 1: debug
	char  echo;     //echo mode, 0: none, 1: echo
	char  fctl;     //flow control, 0: none, 1: hardware, 2: software
	char  parity;   //parity 0: none, 1: odd, 2: even
	char  stopbit;  //stop bits, 1, 2
	const int reserved; //reserved, must be zero
}portinfo_t;

typedef portinfo_t *pportinfo_t;

int  port_open (pportinfo_t pportinfo);
int  port_set  (int fdcom, const pportinfo_t pportinfo);
void port_close(int fdcom);
int  port_send (int fdcom, char *data, int len);
int  port_recv (int fdcom, char *data, int len, int timeout_ms);
#endif
