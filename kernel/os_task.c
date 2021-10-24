#include <stdio.h>
#include <string.h>

#include <os_task.h>
#include <os_fifo.h>

#include <sg_tasks.h>
#include <sg_fifo.h>
#include <os_api.h>
#include <sg_appmodes.h>



OsTaskCtrlType OsTaskCtrlBlk[TASK_ID_MAX];
OsTaskType OsCurrentTask;



/*/
Function: ActivateTask
Parameters:
  TaskID  Task reference.

Description: The task <TaskID> is transferred from the suspended state into the
ready state. The operating system ensures that the task code is being executed
from the first statement.
/*/
StatusType ActivateTask(TaskType TaskID) {
	StatusType stat = E_OK;

	if (TaskID >= TASK_ID_MAX) {
		pr_log("Error: %s() called with invalid TaskID %d\n", __func__, TaskID);
		return E_OS_ID;
	}

	stat = AddTaskToFifoQueue(OsTaskList[TaskID], ReadyQueue);

	return stat;
}


/*/
Function: TerminateTask
Parameters: None
Description: This service causes the termination of the calling task. The 
calling task is transferred from the running state into the suspended state.
/*/
StatusType TerminateTask(void) {
	// TODO: this functions to be redesigned to work on a microcontroller
	// environment by stack manipulation (i.e, saving and restoring uC 
	// registers). In Linux world this is not possible.
	return E_OK;
}


void OsClearActivationsCounts(void) {
	memset(OsTaskCtrlBlk, 0, sizeof(OsTaskCtrlBlk));
}


void OsSetupScheduler(AppModeType mode) {
	int t, m;
	OsFifoType* pFifo;

	if (mode >= OS_MODES_MAX) {
		pr_log("Error: AppMode \"%d >= OS_MODES_MAX\". Task init failed!\n", mode);
		return;
	}

	/* check all tasks marked as autostart */
	for (t=0; t < TASK_ID_MAX; t++) {
		for (m=0; m < OsTaskList[t].n_appmodes; m++) {
			/* do sanity check - for any hand modification of sg code */
			if (t != OsTaskList[t].id) {
				pr_log("Error: %s(), task.id (%d) != id (%d)!",
					__func__, OsTaskList[t].id, t);
				continue; // skip this
			}

			/* check if task 't' is configured to run in this mode */
			if (mode != *(((AppModeType*) OsTaskList[t].appmodes)+m)) {
				continue; // skip this
			}

			/* check if it has already reached activations limit */
			if (OsTaskCtrlBlk[t].activations >= OsTaskList[t].activations) {
				continue; // skip this
			}
			OsTaskCtrlBlk[t].activations++;

			/* all set, we can not add this task to queue */
			AddTaskToFifoQueue(OsTaskList[t], ReadyQueue);
		}
	}
	pr_log("Scheduler setup done!\n");
}


/* The following function is defined in os_alarm.c */
int OsHandleCounters(void);

int OsScheduleTasks(void) {
	OsTaskType* task;
	u32 tick_cnt;
	static u32 tick_cnt_old;
	int i;

	/* Timer / Counter handling */
	tick_cnt = GetOsTickCnt();
	if (tick_cnt != tick_cnt_old) {
		OsHandleCounters();
		tick_cnt_old = tick_cnt;
	}

	/* Task handling */
	for (i = SG_FIFO_QUEUE_MAX_LEN-1; i >= 0; i--) {
		task = GetTaskFromFifoQueue(ReadyQueue, i);
		if (task != NULL) {
			OsCurrentTask = *task;
			OsCurrentTask.handler();
		}
	}
}
