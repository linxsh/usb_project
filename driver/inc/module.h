#ifndef __MODULE_H__
#define __MODULE_H__

typedef enum {
	MOD_DCI_TYPE    = 0,
	MOD_GC0308_TYPE = 1,
} MODULE_TYPE;

typedef struct {
	const char   *name;
	MODULE_TYPE  type;
	int (*step)  (void *args)
	int (*init)  (void);
	int (*deinit)(void);
	int (*open)  (int id);
	int (*close) (int id);
	int (*set)   (int id, int cmd, void *args, int size);
	int (*config)(int id, int cmd, void *args, int size);
	int (*get)   (int id, int cmd, void *args, int size);
} MODULE_OPS_S;

#endif
