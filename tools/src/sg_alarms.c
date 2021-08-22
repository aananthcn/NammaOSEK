#include <stddef.h>
#include <stdbool.h>
#include "sg_alarms.h"
#include "sg_appmodes.h"
#include "sg_tasks.h"
#include "sg_events.h"


#define TRUE    true
#define FALSE    false

/*   A P P M O D E S   F O R   A L A R M S   */
#define ALARM_WAKETASKA_APPMODES_MAX (2)
const AppModeType Alarm_WakeTaskA_AppModes[] = {
	OSDEFAULTAPPMODE,
	MANUFACT_MODE
};


/*   A L A R M S   D E F I N I T I O N S   */
const AppAlarmType AppAlarms_mSecCounter[] = {
	{
		.name = "WakeTaskA",
		.cntr_id = 0,
		.aat = AAT_ACTIVATETASK,
		.aat_arg1 = OS_TASK(Task_A),
		.aat_arg2 = NULL,
		.is_autostart = TRUE,
		.alarmtime = 20,
		.cycletime = 4,
		.n_appmodes = ALARM_WAKETASKA_APPMODES_MAX,
		.appmodes = (const AppModeType **) Alarm_WakeTaskA_AppModes
	},
	{
		.name = "WakeTaskB",
		.cntr_id = 0,
		.aat = AAT_SETEVENT,
		.aat_arg1 = (void*) OS_TASK(Task_B),
		.aat_arg2 = (void*) OS_EVENT(Task_B, event1),
		.is_autostart = FALSE,
		.alarmtime = 0,
		.cycletime = 0,
		.n_appmodes = 0,
		.appmodes = NULL
	}
};

const AppAlarmType AppAlarms_uSecCounter[] = {
	{
		.name = "uSecAlarm",
		.cntr_id = 1,
		.aat = AAT_ALARMCALLBACK,
		.aat_arg1 = Alarm_uSecAlarm_callback,
		.aat_arg2 = NULL,
		.is_autostart = FALSE,
		.alarmtime = 0,
		.cycletime = 0,
		.n_appmodes = 0,
		.appmodes = NULL
	}
};


const AppAlarmCtrlBlockType AppAlarms[] = {
	{
		.alarm = (const AppAlarmType *) &AppAlarms_mSecCounter,
		.len = 2
	},
	{
		.alarm = (const AppAlarmType *) &AppAlarms_uSecCounter,
		.len = 1
	},
};

