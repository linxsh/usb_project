#include <stdio.h>              // printf
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>             // bzero
#include <time.h>
#include "cmd.h"
#include "uart.h"

struct reg_config {
	unsigned char addr;
	unsigned char value;
};

static struct reg_config conf[] = {
};
static int ctrl_c = 0;

static void my_handler(int s)
{
	ctrl_c = 1;
}

static int regs_step(int fdcom, unsigned char *buf, unsigned int len)
{
	int ret;
	UART_FINGER_CMD scmd, acmd;

	scmd.tag = UART_FINGER_TAG_FLAG;
	scmd.opt = FINGER_WRITE;
	scmd.fmt = FINGER_CMD;
	scmd.len = len;
	ret = uart_send_cmd(fdcom, &scmd);
	if (ret != sizeof(scmd)) {
		printf("%s %d: warning (%d)\n", __FUNCTION__, __LINE__, ret);
		return -1;
	}

	memset(&acmd, 0, sizeof(acmd));
	ret = uart_recv_cmd(fdcom, &acmd, 1000);
	if (ret != sizeof(acmd)) {
		printf("%s %d: warning (%d)\n", __FUNCTION__, __LINE__, ret);
		return -1;
	}

	ret = uart_send_data(fdcom, buf, len);
	if (ret != len) {
		printf("%s %d: warning (%d)\n", __FUNCTION__, __LINE__, ret);
		return -1;
	}

	memset(&acmd, 0, sizeof(acmd));
	ret = uart_recv_cmd(fdcom, &acmd, 1000);
	if (ret != sizeof(acmd)) {
		printf("%s %d: warning (%d)\n", __FUNCTION__, __LINE__, ret);
		return -1;
	}

	return 0;
}

static int data_step(int fdcom, unsigned char *buf, unsigned int *len)
{
	int ret;
	UART_FINGER_CMD rcmd, acmd;

	rcmd.tag = UART_FINGER_TAG_FLAG;
	rcmd.opt = FINGER_READ;
	rcmd.fmt = FINGER_CMD;
	rcmd.len = 0;
	ret = uart_send_cmd(fdcom, &rcmd);
	if (ret != sizeof(rcmd)) {
		printf("%s %d: warning (%d)\n", __FUNCTION__, __LINE__, ret);
		return -1;
	}

	memset(&acmd, 0, sizeof(acmd));
	ret = uart_recv_cmd(fdcom, &acmd, 1000);
	if (ret != sizeof(acmd)) {
		printf("%s %d: warning (%d)\n", __FUNCTION__, __LINE__, ret);
		return -1;
	}

	if (acmd.len == 0) {
		sleep(500);
		printf("%s %d: warning (%d)\n", __FUNCTION__, __LINE__, ret);
		return -1;
	}

	rcmd.tag = UART_FINGER_TAG_FLAG;
	rcmd.opt = FINGER_READ;
	rcmd.fmt = FINGER_DATA;
	rcmd.len = acmd.len;
	ret = uart_send_cmd(fdcom, &rcmd);
	if (ret != sizeof(rcmd)) {
		printf("%s %d: warning (%d)\n", __FUNCTION__, __LINE__, ret);
		return -1;
	}

	ret = uart_recv_data(fdcom, buf, acmd.len, 1000);
	if (ret <= 0) {
		printf("%s %d: warning (%d)\n", __FUNCTION__, __LINE__, ret);
		return -1;
	}

	*len = acmd.len;

	return 0;
}

static void data_save(const char *path, unsigned char *buf_addr, unsigned int buf_len)
{
	int ret = 0;
	time_t tt = time(NULL);
	struct tm *stm = localtime(&tt);
	char filename[1024];
	int fd = -1;

	if (path == NULL) {
		sprintf(filename, "./%04d-%02d-%2d-%2d-%2d-%02d.yuv",
				1900 + stm->tm_year,
				1 + stm->tm_mon,
				stm->tm_mday,
				stm->tm_hour,
				stm->tm_min,
				stm->tm_sec);
	} else {
		sprintf(filename, "%s/%04d-%02d-%2d-%2d-%2d-%02d.yuv",
				path,
				1900 + stm->tm_year,
				1 + stm->tm_mon,
				stm->tm_mday,
				stm->tm_hour,
				stm->tm_min,
				stm->tm_sec);
	}

	fd = open(filename, O_WRONLY|O_CREAT, 00755);
	if (fd == -1) {
		printf("file create fail: %s\n", filename);
		return;
	}

	ret = write(fd, buf_addr, buf_len);
	if (ret < buf_len) {
		printf("file arite fail: %s\n", filename);
		return;
	}

	close(fd);

	return;
}

static void parse_conf(char *regfile)
{
	FILE *fd = NULL;
	int readlen = 0;
	size_t len = 0;
	char *str = NULL;

	fd = fopen(regfile, "r");
	while ((readlen = getline(&str, &len, fd)) != -1) {
		unsigned int addr = 0xff, value = 0x0;
		unsigned int i = 0;

		sscanf(str, "0x%x, 0x%x", &addr, &value);
		for (i = 0; i < sizeof(conf)/sizeof(struct reg_config); i++) {
			if (conf[i].addr == addr) {
				conf[i].value = value;
				break;
			}
		}
	}

	free(str);

	fclose(fd);
	return;
}

//*************************Test*********************************
int main(int argc, char *argv[])
{
	int fdcom;
	portinfo_t portinfo = {
		"ttyUSB0",                      // ttyUSB0,...
		'0',                            // print prompt after receiving
		115200,                         // baudrate: 9600
		'8',                            // databit: 8
		'0',                            // debug: off
		'0',                            // echo: off
		'2',                            // flow control: software
		'0',                            // parity: none
		'1',                            // stopbit: 1
		0                          // reserved
	};
	char  opt;
	char *path = NULL, *regfile = NULL;
	struct sigaction sigIntHandler;
	unsigned char buf_addr[2*640*480];
	unsigned int  buf_len = 0;

	while ((opt = getopt(argc,argv,"d:p:")) != -1) {
		switch (opt) {
		case 'd':
			portinfo.tty = strdup(optarg);
			break;
		case 'p':
			path = strdup(optarg);
			break;
		case 'f':
			regfile = strdup(optarg);
			break;
		default:
			exit(1);
		}
	}

	if (regfile)
		parse_conf(regfile);

	fdcom = port_open(&portinfo);
	if (fdcom < 0) {
		printf("Error: open serial port error.\n");
		exit(1);
	}
	port_set(fdcom, &portinfo);

	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	while (regs_step(fdcom, (unsigned char *)conf, sizeof(conf)) == 0) {
		if (ctrl_c) {
			printf("exit regs step..\n");
			exit(1);
		}
	}

	while (data_step(fdcom, buf_addr, &buf_len) == 0) {
		data_save(path, buf_addr, buf_len);
		if (ctrl_c) {
			printf("exit regs step..\n");
			exit(1);
		}
	}

	port_close(fdcom);

	return 0;
}
