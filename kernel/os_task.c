#include <stdio.h>
#include <os_task.h>
#include <os_fifo.h>
#include <sg_tasks.h>
#include <sg_fifo.h>
#include <os_api.h>


OsTaskCtrlType OsTaskCtrlBlk[TASK_ID_MAX];



void SetupScheduler(AppModeType mode) {
	int t, m;
	OsFifoType* pFifo;

	// check all tasks marked as autostart
	for (t=0; t < TASK_ID_MAX; t++) {
		for (m=0; m < OsTaskList[t].n_appmodes; m++) {
			/* do sanity check - for any hand modification of sg code */
			if (t != OsTaskList[t].id) {
				pr_log("Error: %s(), task.id (%d) != id (%d)!",
					__func__, OsTaskList[t].id, t);
			}

			/* check if task 't' is configured to run in this mode */
			if (mode == *(((AppModeType*) OsTaskList[t].appmodes)+m)) {
				AddTaskToFifoQueue(OsTaskList[t], ReadyQueue);
			}
		}
	}
}

int ScheduleTasks(void) {
	OsTaskType* task;
	int i;

	for (i = SG_FIFO_QUEUE_MAX_LEN-1; i >= 0; i--) {
		task = GetTaskFromFifoQueue(ReadyQueue, i);
		if (task != NULL) {
			task->handler();
		}
	}
}