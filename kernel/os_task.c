#include <stdio.h>
#include <string.h>

#include <os_task.h>
#include <os_fifo.h>
#include <os_api.h>

#include <sg_tasks.h>
#include <sg_fifo.h>
#include <sg_appmodes.h>
#include <sg_os_param.h>



OsTaskCtrlType _OsTaskCtrlBlk[TASK_ID_MAX];
OsTaskType _OsCurrentTask;



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

	_OsTaskCtrlBlk[TaskID].state = READY;
	stat = AddTaskToFifoQueue(_OsTaskList[TaskID], ReadyQueue);

	return stat;
}


/*/
Function: TerminateTask
Parameters: None
Description: This service causes the termination of the calling task. The 
calling task is transferred from the running state into the suspended state.
/*/
extern u32 _OsKernelPc;
extern u32 _OsKernelSp;
StatusType TerminateTask(void) {
	_OsTaskCtrlBlk[_OsCurrentTask.id].state = SUSPENDED;
	_set_sp_and_pc(_OsKernelSp, _OsKernelPc);
	/* this call won't reach here, hence no return */
	return E_OK;
}


void OsClearActivationsCounts(void) {
	int t;

	for (t = 0; t < TASK_ID_MAX; t++) {
		_OsTaskCtrlBlk[t].activations = 0;
	}
}

/* _user_stack_top is defined in board-specific linker definition file */
extern u8 _user_stack_top;

void OsSetupScheduler(AppModeType mode) {
	int t, m, tmp;
	OsFifoType* pFifo;
	int sp_acc = 0;

	if (mode >= OS_MODES_MAX) {
		pr_log("Error: AppMode \"%d >= OS_MODES_MAX\". Task init failed!\n", mode);
		return;
	}

	/* check all tasks marked as autostart */
	for (t=0; t < TASK_ID_MAX; t++) {
		for (m=0; m < _OsTaskList[t].n_appmodes; m++) {
			/* do sanity check - for any hand modification of sg code */
			if (t != _OsTaskList[t].id) {
				pr_log("Error: %s(), task.id (%d) != id (%d)! \
				Try do \'build clean\' the re-build code.\n",
					__func__, _OsTaskList[t].id, t);
				continue; // skip this
			}

			/* check if task 't' is configured to run in this mode */
			if (mode != *(((AppModeType*) _OsTaskList[t].appmodes)+m)) {
				continue; // skip this
			}

			/* check if it has already reached activations limit */
			if (_OsTaskCtrlBlk[t].activations >= _OsTaskList[t].activations) {
				continue; // skip this
			}
			_OsTaskCtrlBlk[t].activations++;

			/* all set, we can not add this task to queue */
			AddTaskToFifoQueue(_OsTaskList[t], ReadyQueue);
		}

		/* initialize stack pointer for each tasks */
		tmp = ((int)&_user_stack_top - sp_acc);                   /* top location for stack */
		_OsTaskCtrlBlk[t].sp_top = (intptr_t) (tmp - (tmp % 8));  /* align to 8 */
                tmp = tmp - _OS_CTX_SAVE_SZ;                              /* allow space for context save space */
		_OsTaskCtrlBlk[t].sp_tsk = (intptr_t) (tmp - (tmp % 8));  /* align to 8 */
		_OsTaskCtrlBlk[t].state = SUSPENDED;
		sp_acc += _OsTaskList[t].stack_size + _OS_CTX_SAVE_SZ;    /* find the start of stack for next task */
	}
	pr_log("Scheduler setup done!\n");
}



static inline int OsScheduleCall(OsTaskType* task) {
	static u32 sp_curr;

	/* task is checked by the calling function, hence no check here */
	_OsCurrentTask = *task;
	_OsTaskCtrlBlk[task->id].state = RUNNING;

	if (_OsTaskCtrlBlk[task->id].context_saved) {
		sp_curr = _get_stack_ptr();
		/* continue task from where it was switched before */
		_restore_context(_OsTaskCtrlBlk[task->id].sp_top);
		/* the call shouldn't return here after the above call */

	}
	else {
		/* context not saved, set sp below context space and call */
		sp_curr = _set_stack_ptr(_OsTaskCtrlBlk[task->id].sp_tsk);
		_OsCurrentTask.handler();
	}
	_set_stack_ptr(sp_curr);
	_OsTaskCtrlBlk[task->id].state = SUSPENDED;

	return 0;
}



/* The following function is defined in os_alarm.c */
int OsHandleCounters(void);

int OsScheduleTasks(void) {
	OsTaskType* task;
	u32 tick_cnt;
	static u32 tick_cnt_old;
	int i;

	/* Timer / Counter handling */
	tick_cnt = _GetOsTickCnt();
	if (tick_cnt != tick_cnt_old) {
		OsHandleCounters();
		tick_cnt_old = tick_cnt;
	}

	/* Task handling */
	for (i = SG_FIFO_QUEUE_MAX_LEN-1; i >= 0; i--) {
		task = GetTaskFromFifoQueue(ReadyQueue, i);
		if (task != NULL) {
			OsScheduleCall(task);
		}
	}
}
