#ifndef __CAMERA_H__
#define __CAMERA_H__

typedef enum {
	unsigned char *buf;
	unsigned int  len;
} CAMERA_BUF_CONFIG_T;

typedef enum {
	CMD_COF_CAMERA_BUFFER = 0x0,
} CMD_COF_CAMERA_E;

typedef struct {
} CMD_SET_CAMERA_E;


#endif
