#include <stddef.h>

#include "sg_resources.h"
#include "sg_tasks.h"



/* Resources Definitions in RAM */
ResourceType mutex1;


/*  Resources lists for Tasks */
const TaskType mutex1_tasks[] = {
	TASK_TASK_A_ID,
	TASK_TASK_D_ID,
};

const OsResMapType _OsResList[MAX_RESOURCE_ID] = {
	{
		.res = &mutex1,
		.ceil_prio = 4,
		.n_tasks = 3,
		.task_ids = mutex1_tasks
	},
};
