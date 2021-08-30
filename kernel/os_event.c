#include <osek.h>
#include <os_api.h>

#include <sg_events.h>
#include <sg_tasks.h>


static EventMaskType EventMasks[TASK_ID_MAX];


StatusType SetEvent(TaskType TaskID, EventMaskType Mask) {
	if (TaskID >= TASK_ID_MAX) {
		pr_log("Error: %s() called with invalid TaskID %d\n", __func__, TaskID);
		return E_OS_ID;
	}

	EventMasks[TaskID] |= Mask;
	ActivateTask(TaskID);

	return E_OK; 
}