#ifndef ACN_OSEK_OS_TASK_H
#define ACN_OSEK_OS_TASK_H

#include <osek.h>
#include <sg_tasks.h>

typedef struct {
	u32 activations;
} OsTaskCtrlType;

extern OsTaskCtrlType _OsTaskCtrlBlk[];
extern OsTaskType _OsCurrentTask;

void OsSetupScheduler(AppModeType mode);
int OsScheduleTasks(void);
void OsClearActivationsCounts(void);
int OsInitializeAlarms(AppModeType mode);


#endif