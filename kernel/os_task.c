#include <stdio.h>
#include <string.h>

#include <os_task.h>
#include <os_fifo.h>
#include <os_api.h>

#include <sg_tasks.h>
#include <sg_fifo.h>
#include <sg_appmodes.h>
#include <sg_os_param.h>

#include <zephyr/kernel.h>

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



void OsSetupScheduler(AppModeType mode) {
	int t;

	if (mode >= OS_MODES_MAX) {
		pr_log("Error: AppMode \"%d >= OS_MODES_MAX\". Task init failed!\n", mode);
		return;
	}

	/* check all tasks marked as autostart */
	for (t=0; t < TASK_ID_MAX; t++) {
		/* initialize ceiling priority same as configured priority */
		_OsTaskCtrlBlk[t].ceil_prio = _OsTaskList[t].priority;

                /* do sanity check - for any hand modification of sg code */
                if (t != _OsTaskList[t].id) {
                        pr_log("Error: %s(), task.id (%d) != id (%d)! Try do \
                        a clean build.\n", __func__, _OsTaskList[t].id, t);
                        continue; // skip this
                }

                /* ready tasks if set for autostart */
                if (_OsTaskList[t].autostart) {
                        _OsTaskCtrlBlk[t].state = READY;
                }

                /* all set, we can now create a thread for this task */
                _OsTaskCtrlBlk[t].tid = k_thread_create(&_OsTaskCtrlBlk[t].thread, /* struct k_thread* */
                        _OsStackPtrList[t],                     /* k_thread_stack_t * stack */
                        _OsTaskList[t].stack_size,              /* stack_size */
                        _OsTaskEntryList[t],                    /* k_thread_entry_t entry*/
                        NULL, NULL, NULL,                       /* p1, p2, p3 */
                        K_PRIO_COOP(_OsTaskList[t].priority),   /* priority (smaller == higher in zephyr) */
                        0,                                      /* uint32_t options */
                        K_NO_WAIT                               /* no delay; OsTaskSchedConditionsOk() takes care of OSEK's */
                );
	}
	pr_log("Scheduler setup done!\n");
}



static inline int OsScheduleCall(OsTaskType* task) {
	// u32 sp_curr;

	// /* task is checked by the calling function, hence no check here */
	// _OsCurrentTask = *task;
	// _OsTaskCtrlBlk[task->id].state = RUNNING;

	// if (_OsTaskCtrlBlk[task->id].context_saved) {
	// 	/* continue task from where it was switched before */
	// 	_restore_context(_OsTaskCtrlBlk[task->id].sp_ctx);
	// 	/* The call shouldn't return here after the above call */
	// 	/* instead it should return from the else block below  */
	// }
	// else {
	// 	/* context not saved, set sp below context space and call */
	// 	sp_curr = _set_stack_ptr(_OsTaskCtrlBlk[task->id].sp_tsk);
	// 	_OsCurrentTask.handler();
	// 	_set_stack_ptr(sp_curr);
	// }
	// _OsTaskCtrlBlk[task->id].state = SUSPENDED;

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

	/* Schedule Task in the order of decreasing priority (high --> low) */
	for (i = OS_NO_OF_PRIORITIES-1; i >= 0; i--) {
		task = GetTaskFromFifoQueue(ReadyQueue, _OsTaskValidPriorities[i]);
		if (task != NULL) {
			OsScheduleCall(task);
		}
	}
}



int OsSetCeilingPrio(u32 prio) {
	if (prio > OS_MAX_TASK_PRIORITY) {
		pr_log("Error: %s() called with invalid priority %d\n", __func__, prio);
		return E_OS_ID;
	}

	DisableAllInterrupts();
	_OsTaskCtrlBlk[_OsCurrentTask.id].ceil_prio = prio;
	EnableAllInterrupts();
	return 0;
}



int OsClrCeilingPrio(void) {
	u32 prio;

	DisableAllInterrupts();
	prio = _OsTaskList[_OsCurrentTask.id].priority;
	_OsTaskCtrlBlk[_OsCurrentTask.id].ceil_prio = prio;
	EnableAllInterrupts();
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//                     Thread Control Functions                              //
///////////////////////////////////////////////////////////////////////////////
/*/
    This function will be called by thread entry point functions for each tasks
    defined (auto-generated) by Os-builder in sg_tasks.c, before scheduling tasks
    triggered by Zephyr RTOS's threads.
/*/
bool OsTaskSchedConditionsOk(uint32_t task_id) {
        int m;
        bool retval = FALSE;
        bool appmode_ok = FALSE;
        AppModeType appmode;
        AppModeType *task_app_modes;

        /* input validation */
        if (task_id >= TASK_ID_MAX) {
                printf("ERROR: %s(): input validation failure!\n", __func__);
                return retval;
        }

        /* check if task is configured to run in this mode */
        appmode = GetActiveApplicationMode();
        task_app_modes = ((AppModeType*)_OsTaskList[task_id].appmodes);
	for (m=0; m < _OsTaskList[task_id].n_appmodes; m++) {
                if (appmode == task_app_modes[m]) {
                        appmode_ok = TRUE;
                        break;
                }
        }

        if ((_OsTaskCtrlBlk[task_id].state == READY) && (appmode_ok)) {
                retval = TRUE;
        }

        return retval;
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
	// /* mark the current task as not running */
	// _OsTaskCtrlBlk[_OsCurrentTask.id].state = SUSPENDED;
	
	// /* jump to main loop */
	// _set_sp_and_pc(_OsKernelSp, _OsKernelPc);
	
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
	// if (TaskID >= TASK_ID_MAX) {
	// 	pr_log("Error: %s() called with invalid TaskID %d\n",
	// 		__func__, TaskID);
	// 	return E_OS_ID;
	// }

	// /* mark the current task as not running */
	// _OsTaskCtrlBlk[_OsCurrentTask.id].state = SUSPENDED;

	// /* calling the new tasks and not returning (i.e., jump to main loop)
	// is equivalent to terminating the calling task */
	// OsScheduleCall((OsTaskType*)&_OsTaskList[TaskID]);
	
	// /* jump to main loop */
	// _set_sp_and_pc(_OsKernelSp, _OsKernelPc);
	
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
	// u32 sp_ctx;
	// //pr_log("sp_top: %X\n", _OsTaskCtrlBlk[_OsCurrentTask.id].sp_top);
	// /* save the context of this task, for resuming later */
	// sp_ctx = _save_context(_OsTaskCtrlBlk[_OsCurrentTask.id].sp_top);
	// /* return if this call is resuming from previous context save */
	// if (_OsTaskCtrlBlk[_OsCurrentTask.id].context_saved) {
	// 	_OsTaskCtrlBlk[_OsCurrentTask.id].context_saved = false;
	// 	return E_OK;
	// }
	// //pr_log("sp_ctx: %X\n", sp_ctx);
	// _OsTaskCtrlBlk[_OsCurrentTask.id].sp_ctx = sp_ctx;
	// _OsTaskCtrlBlk[_OsCurrentTask.id].context_saved = true;

	// /* add this task to ready queue, as the scheduler would have removed it */
	// ActivateTask(_OsCurrentTask.id);

	// /* time to stop the current execution */
	// _set_sp_and_pc(_OsKernelSp, _OsKernelPc);
	
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
