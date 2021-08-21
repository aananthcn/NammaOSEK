#include <stdio.h>
#include <osek.h>
#include <os_api.h>

TASK(Task_A) {
	pr_log("%s\n", __func__);
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