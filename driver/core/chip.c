#include "chip.h"

static CHIP_OPS_S *chipOps = NULL;

int core_chip_init(void)
{
#ifdef CHIP_GD32F4XX
	extern CHIP_OPS_S gd32f4xx_ops;
	chipOps = &gd32f4xx_ops;
#endif

	core_log(CHIP, LOG_INFO, "%s %d\n", __FUNCTION__, __LINE__);
	if (chipOps && chipOps->init) {
		return chipOps->init(&chipOps->comp);
	}

	core_log(CHIP, LOG_ERROR, "%s %d\n", __FUNCTION__, __LINE__);
	return -1;
}

int core_chip_deinit(void)
{
	core_log_(CHIP, LOG_INFO, "%s %d\n", __FUNCTION__, __LINE__);
	if (chipOps && chipOps->deinit) {
		return chipOps->deinit(&chipOps->comp);
	}

	core_log(CHIP, LOG_ERROR, "%s %d\n", __FUNCTION__, __LINE__);
	return -1;
}
