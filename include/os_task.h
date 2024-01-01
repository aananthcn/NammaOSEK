#ifndef ACN_OSEK_OS_TASK_H
#define ACN_OSEK_OS_TASK_H

#include <osek.h>
#include <sg_tasks.h>

#include <kernel/thread.h> /* Zephyr kernel */

typedef struct {
	/* Car-OS or OSEK's data */
	u32 activations;
        TaskStateType state;
	u32 ceil_prio;

	/* Zephyr's data */
	k_tid_t tid;
	struct k_thread thread;
} OsTaskCtrlType;

extern OsTaskCtrlType _OsTaskCtrlBlk[];
extern OsTaskType _OsCurrentTask;

void OsSetupScheduler(AppModeType mode);
int OsScheduleTasks(void);
void OsClearActivationsCounts(void);
int OsInitializeAlarms(AppModeType mode);
int OsSetCeilingPrio(u32 prio);
int OsClrCeilingPrio(void);


#endif