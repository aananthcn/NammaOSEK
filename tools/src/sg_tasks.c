#include <stddef.h>
#include "sg_tasks.h"
#include "sg_appmodes.h"
#include "sg_events.h"


/*   T A S K   D E F I N I T I O N   */

const OsTaskType OsTaskList[3] = {
	{
		.app_modes = (const AppModeType **) &Task_A_AppModes,
		.n_app_modes = TASK_A_APPMODE_MAX,
		.evt_msk = NULL,
		.n_evt = TASK_A_EVENT_MAX
	},
	{
		.app_modes = NULL,
		.n_app_modes = TASK_B_APPMODE_MAX,
		.evt_msk = (const EventMaskType**) &Task_B_EventMasks,
		.n_evt = TASK_B_EVENT_MAX
	},
	{
		.app_modes = (const AppModeType **) &Task_C_AppModes,
		.n_app_modes = TASK_C_APPMODE_MAX,
		.evt_msk = (const EventMaskType**) &Task_C_EventMasks,
		.n_evt = TASK_C_EVENT_MAX
	}
};
