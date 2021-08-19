#include <stddef.h>
#include <stdbool.h>
#include "sg_alarms.h"


/*   A L A R M S   D E F I N I T I O N S   */
const AppAlarmType AppAlarms[] = {
	{
		.name = WakeTaskA,
		.cntr_id = 0,
		.aat = AAT_ACTIVATETASK,
	},
	{
		.name = WakeTaskB,
		.cntr_id = 1,
		.aat = AAT_SETEVENT,
	},
	{
		.name = uSecAlarm,
		.cntr_id = 2,
		.aat = AAT_ALARMCALLBACK,
	}
};
