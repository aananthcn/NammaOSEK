#include <stddef.h>
#include <stdbool.h>
#include "sg_alarms.h"


/*   A L A R M S   D E F I N I T I O N S   */
const AppAlarmType AppAlarms[] = {
	{
		.name = WakeTaskA,
		.cntr_id = 0,
		.aat = AAT_ACTIVATETASK,
		.aat_arg1 = Task_A,
		.aat_arg2 = NULL,
		.is_autostart = TRUE,
		.alarmtime = 20,
		.cycletime = 4,
	},
	{
		.name = WakeTaskB,
		.cntr_id = 1,
		.aat = AAT_SETEVENT,
		.aat_arg1 = Task_B,
		.aat_arg2 = event1,
		.is_autostart = FALSE,
		.alarmtime = 0,
		.cycletime = 0,
	},
	{
		.name = uSecAlarm,
		.cntr_id = 2,
		.aat = AAT_ALARMCALLBACK,
		.aat_arg1 = Alarm_uSecAlarm_callback,
		.aat_arg2 = NULL,
		.is_autostart = FALSE,
		.alarmtime = 0,
		.cycletime = 0,
	}
};
