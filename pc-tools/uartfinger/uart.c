#include <fcntl.h>              // open
#include <string.h>             // bzero
#include <stdlib.h>             // exit
#include <fcntl.h>              // open
#include <stdlib.h>             // exit
#include <sys/times.h>          // times
#include <sys/types.h>          // pid_t
#include <termios.h>          //termios, tcgetattr(), tcsetattr()
#include <unistd.h>
#include <sys/ioctl.h>          // ioctl
#include "uart.h"

/*******************************************
 *  波特率转换函数（请确认是否正确）
 ********************************************/
static int _cov_baud(unsigned long int baudrate)
{
	switch(baudrate){
	case 2400:
		return B2400;
	case 4800:
		return B4800;
	case 9600:
		return B9600;
	case 19200:
		return B19200;
	case 38400:
		return B38400;
	case 57600:
		return B57600;
	case 115200:
		return B115200;
	case 460800:
		return B460800;
	case 921600:
		return B921600;
	case 1152000:
		return B1152000;
	case 1500000:
		return B1500000;
	default:
		return B9600;
	}
}

/*******************************************
 *  Setup comm attr
 *  fdcom: 串口文件描述符，pportinfo: 待设置的端口信息（请确认）
 *
 ********************************************/
int port_set(int fdcom, const pportinfo_t pportinfo)
{
	struct termios termios_old, termios_new;
	int     baudrate, tmp;
	char    databit, stopbit, parity, fctl;

	bzero(&termios_old, sizeof(termios_old));
	bzero(&termios_new, sizeof(termios_new));
	cfmakeraw(&termios_new);
	tcgetattr(fdcom, &termios_old);         //get the serial port attributions
	/*------------设置端口属性----------------*/
	//baudrates
	baudrate = _cov_baud(pportinfo -> baudrate);
	cfsetispeed(&termios_new, B115200);        //填入串口输入端的波特率
	cfsetospeed(&termios_new, baudrate);        //填入串口输出端的波特率
	termios_new.c_cflag |= CLOCAL;          //控制模式，保证程序不会成为端口的占有者
	termios_new.c_cflag |= CREAD;           //控制模式，使能端口读取输入的数据

	// 控制模式，flow control
	fctl = pportinfo-> fctl;
	switch(fctl){
	case '0':{
		termios_new.c_cflag &= ~CRTSCTS;        //no flow control
	}break;
	case '1':{
		termios_new.c_cflag |= CRTSCTS;         //hardware flow control
	}break;
	case '2':{
		termios_new.c_iflag |= IXON | IXOFF |IXANY; //software flow control
	}break;
	}

	//控制模式，data bits
	termios_new.c_cflag &= ~CSIZE;      //控制模式，屏蔽字符大小位
	databit = pportinfo -> databit;
	switch(databit){
	case '5':
		termios_new.c_cflag |= CS5;
	case '6':
		termios_new.c_cflag |= CS6;
	case '7':
		termios_new.c_cflag |= CS7;
	default:
		termios_new.c_cflag |= CS8;
	}

	//控制模式 parity check
	parity = pportinfo -> parity;
	switch(parity){
	case '0':{
		termios_new.c_cflag &= ~PARENB;     //no parity check
	}break;
	case '1':{
		termios_new.c_cflag |= PARENB;      //odd check
		termios_new.c_cflag &= ~PARODD;
	}break;
	case '2':{
		termios_new.c_cflag |= PARENB;      //even check
		termios_new.c_cflag |= PARODD;
	}break;
	}

	//控制模式，stop bits
	stopbit = pportinfo -> stopbit;
	if(stopbit == '2'){
		termios_new.c_cflag |= CSTOPB;  //2 stop bits
	}
	else{
		termios_new.c_cflag &= ~CSTOPB; //1 stop bits
	}

	//other attributions default
	termios_new.c_oflag &= ~OPOST;          //输出模式，原始数据输出
	termios_new.c_cc[VMIN]  = 1;            //控制字符, 所要读取字符的最小数量
	termios_new.c_cc[VTIME] = 1;            //控制字符, 读取第一个字符的等待时间    unit: (1/10)second

	tcflush(fdcom, TCIFLUSH);               //溢出的数据可以接收，但不读
	tmp = tcsetattr(fdcom, TCSANOW, &termios_new);  //设置新属性，TCSANOW：所有改变立即生效    tcgetattr(fdcom, &termios_old);

	return(tmp);
}

/*******************************************
 *  Open serial port
 *  tty: 端口号 ttyS0, ttyS1, ....
 *  返回值为串口文件描述符
 ********************************************/
int port_open(pportinfo_t pportinfo)
{
	int fdcom;  //串口文件描述符

	//fdcom = open(ptty, O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY);
	fdcom = open(pportinfo->tty, O_RDWR | O_NOCTTY | O_NONBLOCK);

	return (fdcom);
}

/*******************************************
 *  Close serial port
 ********************************************/
void port_close(int fdcom)
{
	close(fdcom);
}

/********************************************
 *  send data
 *  fdcom: 串口描述符，data: 待发送数据，len: 数据长度
 *  返回实际发送长度
 *********************************************/
int port_send(int fdcom, char *data, int len)
{
	int writelen = 0;

	writelen = write(fdcom, data, len);  //实际写入的长度
	if(writelen == len){
		return (writelen);
	}
	else{
		tcflush(fdcom, TCOFLUSH);
		return -1;
	}
}

/*******************************************
 *  receive data
 *  返回实际读入的字节数
 *
 ********************************************/
int port_recv(int fdcom, char *data, int len, int timeout_ms)
{
	int readlen, fs_sel;
	fd_set  fs_read;
	struct timeval tv_timeout;

	FD_ZERO(&fs_read);
	FD_SET(fdcom, &fs_read);
	tv_timeout.tv_sec  = (timeout_ms / 1000);
	tv_timeout.tv_usec = (timeout_ms % 1000) * 1000;

	fs_sel = select(fdcom+1, &fs_read, NULL, NULL, &tv_timeout);
	if(fs_sel){
		readlen = read(fdcom, data, len);
		return(readlen);
	}
	else{
		return(-1);
	}

	return (readlen);
}
