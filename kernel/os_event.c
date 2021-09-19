#include <osek.h>
#include <os_api.h>
#include <os_task.h>

#include <sg_events.h>
#include <sg_tasks.h>
#include <sg_fifo.h>

#include <stddef.h>


static EventMaskType EventMasks[TASK_ID_MAX];

/*/
Function: SetEvent
Parameters:
  TaskID  Reference to the task for which one or several events are to be set.
  Mask    Mask of the events to be set

Description: The service may be called from an interrupt service routine and 
from the task level, but not from hook routines.
The events of task <TaskID> are set according to the event mask <Mask>. Calling
SetEvent causes the task <TaskID> to be transferred to the ready state, if it 
was waiting for at least one of the events specified in <Mask>.

Particularities: Any events not set in the event mask remain unchanged.
/*/
StatusType SetEvent(TaskType TaskID, EventMaskType Mask) {
	if (TaskID >= TASK_ID_MAX) {
		pr_log("Error: %s() called with invalid TaskID %d\n", __func__,
			TaskID);
		return E_OS_ID;
	}

	EventMasks[TaskID] |= Mask;
	ActivateTask(TaskID);

	return E_OK; 
}


/*/
Function: ClearEvent
Parameters:
  Mask    Mask of the events to be cleared.

Description: The events of the extended task calling ClearEvent are cleared
according to the event mask <Mask>.

Particularities: The system service ClearEvent is restricted to extended tasks
which own the event.
/*/
StatusType ClearEvent(EventMaskType Mask) {
	TaskType task_id;

	task_id = OsCurrentTask.id;
	if (task_id >= TASK_ID_MAX) {
		pr_log("Error: %s() called with invalid TaskID %d\n", __func__,
			task_id);
		return E_OS_ID;
	}

	EventMasks[task_id] &= ~Mask;
	return E_OK;
}


/*/
Function: GetEvent
Parameters:
  TaskID  Task whose event mask is to be returned.
  Event   Reference to the memory of the return data.

Description: This service returns the current state of all event bits of the 
task <TaskID>, not the events that the task is waiting for.
The service may be called from interrupt service routines, task level and some
hook routines

Particularities: The system service ClearEvent is restricted to extended tasks
which own the event.
/*/
StatusType GetEvent(TaskType TaskID, EventMaskRefType Event) {
	if (TaskID >= TASK_ID_MAX) {
		pr_log("Error: %s() called with invalid TaskID %d\n", __func__,
			TaskID);
		return E_OS_ID;
	}

	if (Event == NULL) {
		pr_log("Error: %s() Event pointer is NULL\n", __func__);
		return E_OS_ARG_FAIL;
	}

	*Event = EventMasks[TaskID];
	return E_OK;
}
