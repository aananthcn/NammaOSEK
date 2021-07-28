#include <stddef.h>
#include "sg_tasks.h"


/*   T A S K   D E F I N I T I O N   */

/*  Task AppModes */
const AppModeType Task_A_AppModes[] = {
	OSDEFAULTAPPMODE,
	MANUFACT_MODE,
	HW_TEST_MODE
};
const AppModeType* Task_B_AppModes = NULL;
const AppModeType Task_C_AppModes[] = {
	OSDEFAULTAPPMODE
};

const OsTaskType OsTaskList[3] = {
	{
		.app_modes = (const AppModeType **) &Task_A_AppModes
	},
	{
		.app_modes = NULL
	},
	{
		.app_modes = (const AppModeType **) &Task_C_AppModes
	}
};
