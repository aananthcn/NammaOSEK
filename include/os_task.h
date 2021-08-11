#ifndef ACN_OSEK_OS_TASK_H
#define ACN_OSEK_OS_TASK_H

#include <osek.h>

typedef struct {
	u32 activations;
} OsTaskCtrlType;

extern OsTaskCtrlType OsTaskCtrlBlk[];

void SetupScheduler(AppModeType mode);
int ScheduleTasks(void);
void ClearActivationsCounts(void);


#endif