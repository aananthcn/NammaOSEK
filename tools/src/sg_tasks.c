#include <stddef.h>
#include "sg_tasks.h"
#include "sg_appmodes.h"
#include "sg_events.h"
#include "sg_messages.h"


/*   T A S K   D E F I N I T I O N S   */
const OsTaskType OsTaskList[3] = {
	{
		.appmodes = (const AppModeType **) &Task_A_AppModes,
		.n_appmodes = TASK_A_APPMODE_MAX,
		.evtmsks = NULL,
		.n_evtmsks = TASK_A_EVENT_MAX,
		.msglist = (MessageType**) &Task_A_Messages,
		.n_msglist = TASK_A_MESSAGE_MAX
	},
	{
		.appmodes = NULL,
		.n_appmodes = TASK_B_APPMODE_MAX,
		.evtmsks = (const EventMaskType**) &Task_B_EventMasks,
		.n_evtmsks = TASK_B_EVENT_MAX,
		.msglist = (MessageType**) &Task_B_Messages,
		.n_msglist = TASK_B_MESSAGE_MAX
	},
	{
		.appmodes = (const AppModeType **) &Task_C_AppModes,
		.n_appmodes = TASK_C_APPMODE_MAX,
		.evtmsks = (const EventMaskType**) &Task_C_EventMasks,
		.n_evtmsks = TASK_C_EVENT_MAX,
		.msglist = NULL,
		.n_msglist = TASK_C_MESSAGE_MAX
	}
};
