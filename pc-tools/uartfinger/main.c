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
	{0xfe, 0x00},
	{0x01, 0xfa},
	{0x02, 0x70},
	{0x0f, 0x01},
	{0x03, 0x01},
	{0x04, 0x2c},
	{0xe2, 0x00},  //anti-flicker step [11:8]
	{0xe3, 0x64},  //anti-flicker step [7:0]
	{0xe4, 0x02},  //exp level 1  16.67fps
	{0xe5, 0x58},
	{0xe6, 0x03},  //exp level 2  12.5fps
	{0xe7, 0x20},
	{0xe8, 0x04},  // 0x02},  //exp level 3  8.33fps
	{0xe9, 0xb0},  // 0x58},
	{0xea, 0x09},  //exp level 4  4.00fps
	{0xeb, 0xc4},

//	200*240
/*  {0x05, 0x00},  // row_start_high
	{0x06, 0x8c},  // row_start_low
	{0x07, 0x00},  // col_start_high
	{0x08, 0xc8},  // col_start_low
	{0x09, 0x00},  //[8]cis_win_height  208
	{0x0a, 0xf8},  //[7:0]cis_win_height
	{0x0b, 0x00},  //[9:8]cis_win_width 248
	{0x0c, 0xd0},  //[7:0]cis_win_width
*/

//  240*200
/*  {0x05, 0x00},  // row_start_high
	{0x06, 0xc8},  // row_start_low     // 200
	{0x07, 0x00},  // col_start_high
	{0x08, 0x8c},  // col_start_low     // 140
	{0x09, 0x00},  //[8]cis_win_height   208
	{0x0a, 0xd0},  //[7:0]cis_win_height
	{0x0b, 0x00},  //[9:8]cis_win_width  248
	{0x0c, 0xf8},  //[7:0]cis_win_width
	*/

// 480*100 原始640*480
/*  {0x05, 0x00},  // row_start_high
	{0x06, 0x50},  // row_start_low      //
	{0x07, 0x00},  // col_start_high
	{0x08, 0xbe},  // col_start_low      //
	{0x09, 0x00},  //[8]cis_win_height
	{0x0a, 0x6c},  //[7:0]cis_win_height //  0x64+8 配置后+8
	{0x0b, 0x01},  //[9:8]cis_win_width
	{0x0c, 0xe8},  //[7:0]cis_win_width  //  0x1e+8 配置后+8
*/

	//   480*480
/*  {0x05, 0x00},  // row_start_high
	{0x06, 0x00},  // row_start_low
	{0x07, 0x00},  // col_start_high
	{0x08, 0x00},  // col_start_low
	{0x09, 0x01},  //[8]cis_win_height
	{0x0a, 0xe8},  //[7:0]cis_win_height
	{0x0b, 0x01},  //[9:8]cis_win_width
	{0x0c, 0xe8},  //[7:0]cis_win_width
*/

//   640*480
/*  {0x05, 0x00},  // row_start_high
	{0x06, 0x00},  // row_start_low
	{0x07, 0x00},  // col_start_high
	{0x08, 0x00},  // col_start_low
	{0x09, 0x01},  //[8]cis_win_height
	{0x0a, 0xe8},  //[7:0]cis_win_height
	{0x0b, 0x02},  //[9:8]cis_win_width
	{0x0c, 0x88},  //[7:0]cis_win_width
*/

// 360*360
/*  {0x05, 0x00},  // row_start_high
	{0x06, 0x8c},  // row_start_low
	{0x07, 0x00},  // col_start_high
	{0x08, 0x3c},  // col_start_low
	{0x09, 0x01},  //[8]cis_win_height
	{0x0a, 0x70},  //[7:0]cis_win_height
	{0x0b, 0x01},  //[9:8]cis_win_width
	{0x0c, 0x70},  //[7:0]cis_win_width
*/

// 300*300
	{0x05, 0x00},  // row_start_high
	{0x06, 0x00},  // row_start_low
	{0x07, 0x00},  // col_start_high
	{0x08, 0x00},  // col_start_low
	{0x09, 0x01},  //[8]cis_win_height
	{0x0a, 0x34},  //[7:0]cis_win_height
	{0x0b, 0x01},  //[9:8]cis_win_width
	{0x0c, 0x34},  //[7:0]cis_win_width


// 240*240
/*	{0x05, 0x00},  // row_start_high
	{0x06, 0xc8},  // row_start_low
	{0x07, 0x00},  // col_start_high
	{0x08, 0x78},  // col_start_low
	{0x09, 0x00},  //[8]cis_win_height
	{0x0a, 0xf8},  //[7:0]cis_win_height
	{0x0b, 0x00},  //[9:8]cis_win_width
	{0x0c, 0xf8},  //[7:0]cis_win_width
*/

	{0x0d, 0x02},  //vs_st
	{0x0e, 0x02},  //vs_et
	{0x10, 0x26},  //[7:4]restg_width, [3:0]sh_width
	{0x11, 0x0d},  //fd//[7:4]tx_width, [3:0]space width,*2
	{0x12, 0x2a},  //sh_delay
	{0x13, 0x00},  //[3:0] row_tail_width
	{0x14, 0x10},  //[7]hsync_always ,[6] NA,  [5:4] CFA sequence
	{0x15, 0x0a},  //[7:6]output_mode,,[5:4]restg_mode,[3:2]sdark_mode, [1]new exposure,[0]badframe_en
	{0x16, 0x05},  //[7:5]NA, [4]capture_ad_data_edge, [3:0]Number of A/D pipe stages 
	{0x17, 0x01},  //[7:6]analog_opa_r,[5]coltest_en, [4]ad_test_enable, 
	{0x18, 0x44},  //[7]NA,  [6:4]column gain ee, [3]NA, [2:0]column gain eo
	{0x19, 0x44},  //[7]NA,  [6:4]column gain oe, [3]NA, [2:0]column gain oo
	{0x1a, 0x2a},  //[3:2]colg_r column gain opa bias current, [1]clk_delay, [0] apwd
	{0x1b, 0x00},  //[7:2]NA, [1:0]BIN4 AND BIN2
	{0x1c, 0x49},  //c1//[7]hrst_enbale, [6:4]da_rsg, [3]tx high enable, [2]NA, [1:0]da18_r 
	{0x1d, 0x9a},  //08//[7]vref_en, [6:4]da_vef, [3]da25_en, [2]NA, [1:0]da25_r,set da25 voltage
	{0x1e, 0x61},  //60//[7]LP_MTD,[6:5]opa_r,ADC's operating current,  [4:2]NA, [1:0]sref
	{0x1f, 0x15},  //[7:6]NA, [5:4]sync_drv, [3:2]data_drv, [1:0]pclk_drv
	{0x20, 0xef},  //[7]bks[6]gamma[5]cc[4]ee[3]intp[2]dn[1]dd[0]lsc
	{0x21, 0xf8},  //[7]na[6]na[5]skin_ee[4]cbcr_hue_en[3]y_as_en[2]auto_gray_en[1]y_gamma_en[0]na
	{0x22, 0x57},  //[7]na [6]auto_dndd [5]auto_ee [4]auto_sa [3]na [2]abs [1]awb  [0]na
	{0x24, 0xb1},  //YCbYcr //  a2
	{0x25, 0x0f},
	{0x26, 0x03},  //02		// 07-16原始值01
	{0x2f, 0x01},  //debug mode3
	{0x30, 0xf7},  //blk mode [7]dark current mode:1 use exp rated dark ,0 use ndark row calculated  
	{0x31, 0x50},  //blk_value limit.64 low align to 11bits;8 for 256 range
	{0x32, 0x00},  //global offset
	{0x39, 0x04},  // exp_ate_darkc
	{0x3a, 0x20},  //{7:6}offset submode {5:0}offset ratio
	{0x3b, 0x20},  //{7:6}darkc submode {5:0}dark current ratio
	{0x3c, 0x00},  //manual g1 offset
	{0x3d, 0x00},  //manual r offset
	{0x3e, 0x00},  //manual b offset
	{0x3f, 0x00},  //manual g2 offset
	{0x50, 0x14},  //10  //global gain
	{0x53, 0x80},  //G 
	{0x54, 0x80},  //R channel gain
	{0x55, 0x80},  //B channel gain
	{0x56, 0x80},
	{0x8b, 0x20},  //r2
	{0x8c, 0x20},  //g2
	{0x8d, 0x20},  //b2
	{0x8e, 0x14},  //r4
	{0x8f, 0x10},  //g4
	{0x90, 0x14},  //b4
	{0x91, 0x3c},  //[7]singed4 [6:0]row_cneter
	{0x92, 0x50},  //col_center
	{0x5d, 0x12},  //decrease 1
	{0x5e, 0x1a},  //decrease 2
	{0x5f, 0x24},  //decrease 3
	{0x60, 0x07},  //[4]zero weight mode
	{0x61, 0x15},  //[7:6]na
	{0x62, 0x20},  //b base		//0x08
	{0x64, 0x03},  //[7:4]n base [3:0]c weight
	{0x66, 0xe8},  //dark_th ,bright_th
	{0x67, 0x86},  //flat high, flat low
	{0x68, 0xa2},  //[7:4]dd limit [1:0]dd ratio
	{0x69, 0x18},  //gain high th
	{0x6a, 0x0f},  //[7:4]dn_c slop          //[3]use post_gain [2]use pre_gain [1]use global gain [0]use col gain
	{0x6b, 0x00},  //[7:4]dn_b slop [3:0]dn_n slop
	{0x6c, 0x5f},  //[7:4]bright_th start [3:0]bright_th slop
	{0x6d, 0x8f},  //[7:4]dd_limit_start[3:0]dd_limit slop
	{0x6e, 0x55},  //[7:4]ee1 effect start [3:0]slope  broad
	{0x6f, 0x38},  //[7:4]ee2 effect start [3:0]slope  narrow
	{0x70, 0x15},  //saturation dec slope
	{0x71, 0x33},  //[7:4]low limit,[3:0]saturation slope
	{0x72, 0xdc},  //[7]edge_add_mode [6]new edge mode [5]edge2_mode [4]HP_mode
	{0x73, 0x80},  //[7]edge_add_mode2 [6]NA [5]only 2direction [4]fixed direction th
	{0x74, 0x02},  //direction th1
	{0x75, 0x3f},  //direction th2
	{0x76, 0x02},  //direction diff th      h>v+diff ; h>th1 ; v<th2
	{0x77, 0x11},  //[7:4]edge1_effect [3:0]edge2_effect  0x36
	{0x78, 0x88},  //[7:4]edge_pos_ratio  [3:0]edge neg ratio
	{0x79, 0x81},  //edge1_max,edge1_min
	{0x7a, 0x81},  //edge2_max,edge2_min
	{0x7b, 0x22},  //edge1_th,edge2_th
	{0x7c, 0xff},  //pos_edge_max,neg_edge_max
	{0x93, 0x48},  // <--40
	{0x94, 0x02},
	{0x95, 0x07},
	{0x96, 0xe0},
	{0x97, 0x40},
	{0x98, 0xf0},
	{0xb1, 0x40},  //manual cb
	{0xb2, 0x40},  //manual cr
	{0xb3, 0x50},  // x040
	{0xb6, 0xe0},
	{0xbd, 0x38},
	{0xbe, 0x36},  // [5:4]gray mode 00:4&8  01:4&12 10:4&20  11:8$16   [3:0] auto_gray
	{0xd0, 0xcb},  // exp is gc mode
	{0xd1, 0x10},  //every N
	{0xd2, 0x10},  // 7 aec enable 5 clore y mode 4skin weight 3 weight drop mode
	{0xd3, 0x48},  //Y_target and low pixel thd high X4 low X2
	{0xd5, 0xf2},  //lhig
	{0xd6, 0x16},  // ignore mode
	{0xdb, 0x92},
	{0xdc, 0xa5},  //fast_margin  fast_ratio
	{0xdf, 0x23},  // I_fram D_ratio
	{0xd9, 0x00},  // colore offset in CAL ,now is too dark so set zero
	{0xda, 0x00},  // GB offset
	{0xe0, 0x09},
	{0xed, 0x04},  //minimum exposure low 8  bits
	{0xee, 0xa0},  //max_post_dg_gain
	{0xef, 0x40},  //max_pre_dg_gain
	{0x80, 0x03},
	{0x80, 0x03},
	{0x9F, 0x10},
	{0xA0, 0x20},
	{0xA1, 0x38},
	{0xA2, 0x4E},
	{0xA3, 0x63},
	{0xA4, 0x76},
	{0xA5, 0x87},
	{0xA6, 0xA2},
	{0xA7, 0xB8},
	{0xA8, 0xCA},
	{0xA9, 0xD8},
	{0xAA, 0xE3},
	{0xAB, 0xEB},
	{0xAC, 0xF0},
	{0xAD, 0xF8},
	{0xAE, 0xFD},
	{0xAF, 0xFF},
	{0xc0, 0x00},  //Y_gamma_0
	{0xc1, 0x10},  //Y_gamma_1
	{0xc2, 0x1C},  //Y_gamma_2
	{0xc3, 0x30},  //Y_gamma_3
	{0xc4, 0x43},  //Y_gamma_4
	{0xc5, 0x54},  //Y_gamma_5
	{0xc6, 0x65},  //Y_gamma_6
	{0xc7, 0x75},  //Y_gamma_7
	{0xc8, 0x93},  //Y_gamma_8
	{0xc9, 0xB0},  //Y_gamma_9
	{0xca, 0xCB},  //Y_gamma_10
	{0xcb, 0xE6},  //Y_gamma_11
	{0xcc, 0xFF},  //Y_gamma_12
	{0xf0, 0x02},
	{0xf1, 0x01},
	{0xf2, 0x02},  //anual stretch K
	{0xf3, 0x30},  //he limit of Y_stretch
	{0xf7, 0x12},
	{0xf8, 0x0a},
	{0xf9, 0x9f},
	{0xfa, 0x78},
	{0xfe, 0x01},
	{0x00, 0xf5},  //high_low limit
	{0x02, 0x20},  //y2c
	{0x04, 0x10},
	{0x05, 0x08},
	{0x06, 0x20},
	{0x08, 0x0a},
	{0x0a, 0xa0},  // number limit
	{0x0b, 0x60},  // skip_mode
	{0x0c, 0x08},  
	{0x0e, 0x44},  // width step
	{0x0f, 0x32},  // height step
	{0x10, 0x41},  
	{0x11, 0x37},  // 0x3f
	{0x12, 0x22},  
	{0x13, 0x19},  //13//smooth 2
	{0x14, 0x44},  //R_5k_gain_base
	{0x15, 0x44},  //B_5k_gain_base
	{0x16, 0xc2},  //c2//sinT
	{0x17, 0xa8},  //ac//a8//a8//a8//cosT
	{0x18, 0x18},  //X1 thd
	{0x19, 0x50},  //X2 thd
	{0x1a, 0xd8},  //e4//d0//Y1 thd
	{0x1b, 0xf5},  //Y2 thd
	{0x70, 0x40},  // A R2G low	
	{0x71, 0x58},  // A R2G high
	{0x72, 0x30},  // A B2G low
	{0x73, 0x48},  // A B2G high
	{0x74, 0x20},  // A G low
	{0x75, 0x60},  // A G high  
	{0x77, 0x20},
	{0x78, 0x32},
	{0x30, 0x03},  //[1]HSP_en [0]sa_curve_en
	{0x31, 0x40},
	{0x32, 0x10},
	{0x33, 0xe0},
	{0x34, 0xe0},
	{0x35, 0x00},
	{0x36, 0x80},
	{0x37, 0x00},
	{0x38, 0x04},  //sat1, at8   
	{0x39, 0x09},
	{0x3a, 0x12},
	{0x3b, 0x1C},
	{0x3c, 0x28},
	{0x3d, 0x31},
	{0x3e, 0x44},
	{0x3f, 0x57},
	{0x40, 0x6C},
	{0x41, 0x81},
	{0x42, 0x94},
	{0x43, 0xA7},
	{0x44, 0xB8},
	{0x45, 0xD6},
	{0x46, 0xEE},  //sat15,at224
	{0x47, 0x0d},  //blue_edge_dec_ratio  
	{0xfe, 0x00},
};

static int ctrl_c = 0;

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

static void data_save(const char *path, unsigned char *buf_addr, unsigned int buf_len)
{
	int ret = 0;
	time_t tt = time(NULL);
	struct tm *stm = localtime(&tt);
	char filename[1024];
	int fd = -1;

	if (path == NULL) {
		sprintf(filename, "./%04d.%02d.%02d-%02d.%02d.%02d.yuv",
				1900 + stm->tm_year,
				1 + stm->tm_mon,
				stm->tm_mday,
				stm->tm_hour,
				stm->tm_min,
				stm->tm_sec);
	} else {
		sprintf(filename, "%s/%04d.%02d.%02d-%02d.%02d.%02d.yuv",
				path,
				1900 + stm->tm_year,
				1 + stm->tm_mon,
				stm->tm_mday,
				stm->tm_hour,
				stm->tm_min,
				stm->tm_sec);
	}

	printf("save file: %s\n", filename);
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

	printf("Recieve Len: %d\n", acmd.len);
	if (acmd.len == 0) {
		printf("%s %d: warning (%d)\n", __FUNCTION__, __LINE__, ret);
		return 0;
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

	while (regs_step(fdcom, (unsigned char *)conf, sizeof(conf)) != 0) {
		if (ctrl_c) {
			printf("exit regs step..\n");
			exit(1);
		}
	}

	while (data_step(fdcom, buf_addr, &buf_len) == 0) {
		if (buf_len > 0)
			data_save(path, buf_addr, buf_len);
		if (ctrl_c) {
			printf("exit regs step..\n");
			exit(1);
		}
		sleep(1);
	}

	port_close(fdcom);

	return 0;
}
