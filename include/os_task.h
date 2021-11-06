#ifndef ACN_OSEK_OS_TASK_H
#define ACN_OSEK_OS_TASK_H

#include <osek.h>
#include <sg_tasks.h>

typedef struct {
	u32 activations;
	u32 sp_top;
	u32 sp_tsk;
        TaskStateType state;
	bool context_saved; /* this will be true for schedule() and premption cases */
} OsTaskCtrlType;

extern OsTaskCtrlType _OsTaskCtrlBlk[];
extern OsTaskType _OsCurrentTask;

void OsSetupScheduler(AppModeType mode);
int OsScheduleTasks(void);
void OsClearActivationsCounts(void);
int OsInitializeAlarms(AppModeType mode);


#endif