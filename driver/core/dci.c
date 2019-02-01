#include "module.h"
#include "dci.h"

static MODULE_OPS_S *dciOps = NULL;

void core_dci_register(MODULE_OPS_S *ops)
{
	dciOps = ops;
}

void core_dci_cancel(void)
{
	dciOps = NULL;
}

int core_dci_open(int id)
{
	if (dciOps && dciOps->open)
		dciOps->open(id);

	return 0;
}

int core_dci_close(int id)
{
	if (dciOps && dciOps->close)
		dciOps->close(id);

	return 0;
}
