#ifndef ACN_OSEK_OS_TASK_H
#define ACN_OSEK_OS_TASK_H

#include <osek.h>
#include <sg_tasks.h>

// #include <kernel/thread.h> /* Zephyr kernel */

typedef struct {
	u32 activations;
	u32 sp_top; // TODO: Remove stack pointer stuff - sp_top
	u32 sp_ctx; // TODO: Remove stack pointer stuff - sp_ctx
	u32 sp_tsk; // TODO: Remove stack pointer stuff - sp_tsk
        TaskStateType state;
	bool context_saved; /* TODO: remove this, this will be true for schedule() and premption cases. */
	u32 ceil_prio;
	// k_tid_t kthread_id;
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