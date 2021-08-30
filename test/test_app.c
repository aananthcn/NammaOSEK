#include <stdio.h>
#include <osek.h>
#include <os_api.h>

TASK(Task_A) {
	AlarmBaseType info;
	pr_log("%s\n", __func__);

#ifdef ALARM_BASE_TEST
	if (E_OK == GetAlarmBase(0, &info))
		pr_log("0: ticks/base = %d\n", info.ticksperbase);
	if (E_OK == GetAlarmBase(1, &info))
		pr_log("1: ticks/base = %d\n", info.ticksperbase);
	if (E_OK == GetAlarmBase(2, &info))
		pr_log("2: ticks/base = %d\n", info.ticksperbase);
#endif
}

TASK(Task_B) {
	pr_log("%s\n", __func__);
}

TASK(Task_C) {
	pr_log("%s\n", __func__);
}

void Alarm_uSecAlarm_callback(void) {
	pr_log("%s\n", __func__);
}