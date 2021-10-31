#include <stdio.h>
#include <osek.h>
#include <os_api.h>

#define GETEVENT_TEST

TASK(Task_A) {
	AlarmBaseType info;
	TickType tick_left;
	static bool cycle_started = false;
	pr_log("%s\n", __func__);

#ifdef ALARM_BASE_TEST
	if (E_OK == GetAlarmBase(0, &info))
		pr_log("0: ticks/base = %d\n", info.ticksperbase);
	if (E_OK == GetAlarmBase(1, &info))
		pr_log("1: ticks/base = %d\n", info.ticksperbase);
	if (E_OK == GetAlarmBase(2, &info))
		pr_log("2: ticks/base = %d\n", info.ticksperbase);
#endif

#ifdef GET_ALARM_TEST
	if (E_OK == GetAlarm(0, &tick_left))
		pr_log("0: ticks remaining = %d\n", tick_left);
#endif

#ifdef SET_REL_ALARM_TEST
	if (!cycle_started) {
		SetRelAlarm(0, 250, 2000);
		cycle_started = true;
	}
#endif

#ifdef SET_ABS_ALARM_TEST
	SetAbsAlarm(1, 5000, 1500); // start at 5th sec and repeat every 1.5 sec
#endif

#ifdef CANCEL_ALARM_TEST
	SetAbsAlarm(1, 1, 1); // start at 5th sec and repeat every 1.5 sec
#endif

#ifdef EVENT_SET_CLEAR_TEST
	static int setcnt = 4;
	if (setcnt > 0) {
		setcnt--;
		SetEvent(1, 1);
	}
#endif

#ifdef GETEVENT_TEST
	static bool toggle_bit;
	EventMaskType Event;
	if (toggle_bit) {
		toggle_bit = false;
		SetEvent(1, 0x101);
		GetEvent(1, &Event);
		printf("Task A: Event = %lX\n", Event);
	}
	else {
		toggle_bit = true;
	}
#endif
}

TASK(Task_B) {
	pr_log("%s\n", __func__);
#ifdef CANCEL_ALARM
	static int i = 10;
	if (i-- <= 0) 
		CancelAlarm(1);
#endif
#ifdef EVENT_SET_CLEAR_TEST
	ClearEvent(1);
#endif
#ifdef GETEVENT_TEST
	EventMaskType Event;
	ClearEvent(0x101);
	GetEvent(1, &Event);
	printf("Task B: Event = %lX\n", Event);
#endif
}

TASK(Task_C) {
	pr_log("%s\n", __func__);
}

void Alarm_uSecAlarm_callback(void) {
	pr_log("%s\n", __func__);
}