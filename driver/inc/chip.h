#ifndef __CHIP_H__
#define __CHIP_H__

#include "module.h"

typedef enum {
	GD32F4XX_TYPE,
} CHIP_TYPE;

typedef struct {
	int          count;
	MODULE_OPS_S *ops[MAX_CHIP_COMP];
	MODULE_RES_S *res[MAX_CHIP_COMP];
} CHIP_COMP;

typedef struct {
#define MAX_CHIP_COMP (16)
	const char   *name;
	CHIP_TYPE    type;
	CHIP_COMP    comp;
	int (*init)  (CHIP_COMP *comp);
	int (*deinit)(CHIP_COMP *comp);
} CHIP_OPS_S;

#endif
