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



///////////////////////////////////////////////////////////////////////////////
//                       OS Internal Functions                               //
///////////////////////////////////////////////////////////////////////////////
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
	u32 sp_curr;

	/* task is checked by the calling function, hence no check here */
	_OsCurrentTask = *task;
	_OsTaskCtrlBlk[task->id].state = RUNNING;

	if (_OsTaskCtrlBlk[task->id].context_saved) {
		/* continue task from where it was switched before */
		_restore_context(_OsTaskCtrlBlk[task->id].sp_top);
		/* The call shouldn't return here after the above call */
		/* instead it should return from the else block below  */
	}
	else {
		/* context not saved, set sp below context space and call */
		sp_curr = _set_stack_ptr(_OsTaskCtrlBlk[task->id].sp_tsk);
		_OsCurrentTask.handler();
		_set_stack_ptr(sp_curr);
	}
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



///////////////////////////////////////////////////////////////////////////////
//                          Public Functions                                 //
///////////////////////////////////////////////////////////////////////////////
/*/
Function: ActivateTask
Parameters:
  TaskID  Task reference.

Description: The task <TaskID> is transferred from the suspended state into the
             ready state. The operating system ensures that the task code is
	     being executed from the first statement.
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



/* Following global variables are used by TerminateTask and ChainTask APIs */
extern u32 _OsKernelPc;
extern u32 _OsKernelSp;

/*/
Function: TerminateTask
Parameters: None
Description: This service causes the termination of the calling task. The 
             calling task is transferred from the running state into the
	     suspended state.
/*/
StatusType TerminateTask(void) {
	/* mark the current task as not running */
	_OsTaskCtrlBlk[_OsCurrentTask.id].state = SUSPENDED;
	
	/* jump to main loop */
	_set_sp_and_pc(_OsKernelSp, _OsKernelPc);
	
	/* this call won't reach here, but just to satisfy the compiler */
	return E_OK;
}



/*/
Function: ChainTask
Parameters: TaskID
Description: This service causes the termination of the calling task. After
   	     termination of the calling task a succeeding task <TaskID> is
	     activated. Using this service, it ensures that the succeeding task
	     starts to run at the earliest after the calling task has been
	     terminated.
/*/
StatusType ChainTask(TaskType TaskID) {
	if (TaskID >= TASK_ID_MAX) {
		pr_log("Error: %s() called with invalid TaskID %d\n",
			__func__, TaskID);
		return E_OS_ID;
	}

	/* mark the current task as not running */
	_OsTaskCtrlBlk[_OsCurrentTask.id].state = SUSPENDED;

	/* calling the new tasks and not returning (i.e., jump to main loop)
	is equivalent to terminating the calling task */
	OsScheduleCall((OsTaskType*)&_OsTaskList[TaskID]);
	
	/* jump to main loop */
	_set_sp_and_pc(_OsKernelSp, _OsKernelPc);
	
	/* this call won't reach here, but just to satisfy the compiler */
	return E_OK;
}



/*/
Function: Schedule 
Parameters: None
Description: If a higher-priority task is ready, the internal resource of the
             task is released, the current task is put into the ready state,
	     its context is saved and the higher-priority task is executed.
	     Otherwise the calling task is continued.
/*/
StatusType Schedule(void) {
	/* save the context of this task, for resuming later */
	_save_context(_OsTaskCtrlBlk[_OsCurrentTask.id].sp_top);
	/* return if this call is resuming from previous context save */
	if (_OsTaskCtrlBlk[_OsCurrentTask.id].context_saved) {
		_OsTaskCtrlBlk[_OsCurrentTask.id].context_saved = false;
		return E_OK;
	}
	_OsTaskCtrlBlk[_OsCurrentTask.id].context_saved = true;

	/* add this task to ready queue, as the scheduler would have removed it */
	ActivateTask(_OsCurrentTask.id);

	/* time to stop the current execution */
	_set_sp_and_pc(_OsKernelSp, _OsKernelPc);
	
	/* control never reaches this line */
	return E_OK;
}



/*/
Function: GetTaskID
Parameters: pointer to TaskID
Description: GetTaskID returns the information about the TaskID of the task
	     which is currently running.
/*/
StatusType GetTaskID(TaskRefType pTaskID) {
	if (pTaskID == NULL) {
		pr_log("Error: %s() taskID reference is NULL\n", __func__);
		return E_OS_ARG_FAIL;
	}
	*pTaskID = _OsCurrentTask.id;

	return E_OK;
}



/*/
Function: GetTaskState
Parameters: arg1: TaskID, arg2: pointer to State
Description: Returns the state of a task (running, ready, waiting, suspended)
	     at the time of calling GetTaskState.
/*/
StatusType GetTaskState(TaskType TaskID, TaskStateRefType pState) {
	if (TaskID >= TASK_ID_MAX) {
		pr_log("Error: %s() called with invalid TaskID %d\n",
			__func__, TaskID);
		return E_OS_ID;
	}

	if (pState == NULL) {
		pr_log("Error: %s() taskID reference is NULL\n", __func__);
		return E_OS_ARG_FAIL;
	}
	*pState = _OsTaskCtrlBlk[TaskID].state;

	return E_OK;
}
