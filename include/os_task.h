#ifndef ACN_OSEK_OS_TASK_H
#define ACN_OSEK_OS_TASK_H

#include <osek.h>

typedef struct {
	u32 activations;
} OsTaskCtrlType;

extern OsTaskCtrlType OsTaskCtrlBlk[];

void OsSetupScheduler(AppModeType mode);
int OsScheduleTasks(void);
void OsClearActivationsCounts(void);
int OsInitializeAlarms(AppModeType mode);


#endif