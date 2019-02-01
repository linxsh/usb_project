#include "chip.h"
#include "module.h"
#include "gd32f4xx_dci_ops.h"

#define MODULE_STEP(ops, res) do {          \
	if (ops && ops->step)                   \
		ops->step(res);                     \
} while(0)

#define MODULE_INIT(ops)  do {              \
	if (ops && ops->init)                   \
		ops->init();                        \
} while(0)

#define MODULE_DEINIT(ops)  do {            \
	if (ops && ops->deinit)                 \
		ops->deinit();                      \
} while(0)


static int _install(CHIP_COMP *comp)
{
	extern MODULE_OPS_S gd32f4xx_dci_ops;
	core_dci_register(&gd32f4xx_dci_ops);
	comp->ops[comp->count++] = &gd32f4xx_dci_ops;
}

static int _uninstall(CHIP_COMP *comp)
{
	core_dci_cancel();

	for (i = 0; i < comp->count; i++)
		comp->ops[i] = NULL;

	return 0;
}

static int gd32f4xx_init(CHIP_COMP *comp)
{
	unsigned int i = 0;

	comp->count = 0;
	_install(comp);

	for (i = 0; i < comp->count; i++) {
		MODULE_STEP(comp.ops[i], comp.res[i]);
	}

	for (i = 0; i < comp->count; i++) {
		MODULE_INIT(comp.ops[i]);
	}

	return 0;
}

static int gd32f4xx_deinit(CHIP_COMP *comp)
{
	unsigned int i = 0;

	for (i = 0; i < comp->count; i++) {
		MODULE_DEINIT(comp.ops[i]);
	}

	_uninstall(comp);
	comp->count = 0;

	return 0;
}

CHIP_OPS_S gd32f4xx_ops = {
	.name   = "gd32f4xx",
	.type   = GD32F4XX_TYPE,
	.init   = gd32f4xx_init,
	.deinit = gd32f4xx_deinit,
};
