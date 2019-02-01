#ifndef __MODULE_H__
#define __MODULE_H__

typedef enum {
	MODULE_DCI_TYPE = 0,
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
