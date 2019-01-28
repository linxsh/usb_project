#ifndef __GC_0308_HAL_H__
#define __GC_0308_HAL_H__

#include "../inc/camera.h"

extern void hal_gc0308_init(void);
extern void hal_gc0308_deinit(void);
extern int  hal_gc0308_set(CMD_SET_CAMERA_T cmd, void *args);
extern int  hal_gc0308_enable(void);
extern int  hal_gc0308_finish(void);
extern int  hal_gc0308_config_buffer(uint8_t *buf, uint32_t len);

#endif
