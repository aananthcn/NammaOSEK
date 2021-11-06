#include <stdio.h>
#include <osek.h>
#include <os_api.h>

#define GETEVENT_TEST
#define GETTASKID_GETTASKSTATE_TEST

TASK(Task_A) {
	AlarmBaseType info;
	TickType tick_left;
	static bool cycle_started = false;
	pr_log("%s, sp=0x%08X\n", __func__, _get_stack_ptr());

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

#ifdef TERMINATE_TASK_TEST
	TerminateTask();
#endif

#ifdef WAITEVENT_TEST
	WaitEvent(1);
#endif

#ifdef CHAINTASK_TEST
	ChainTask(2);
#endif

#ifdef SCHEDULE_TEST
	Schedule();
#endif

#ifdef GETTASKID_GETTASKSTATE_TEST
	TaskType task;
	TaskStateType state;
	GetTaskID(&task);
	GetTaskState(task, &state);
	pr_log("Current task: %d, state = %d\n", task, state);
	task = 1;
	GetTaskState(task, &state);
	pr_log("Task: %d, state = %d\n", task, state);
	task = 2;
	GetTaskState(task, &state);
	pr_log("Task: %d, state = %d\n", task, state);
#endif

	static bool toggle_bit;
	EventMaskType Event = 0;
	if (toggle_bit) {
		toggle_bit = false;
		SetEvent(1, 0x101);
		GetEvent(1, &Event);
		pr_log("Task A: Event = 0x%016X\n", Event);
	}
	else {
		toggle_bit = true;
	}
#endif
}

TASK(Task_B) {
	pr_log("%s, sp=0x%08X\n", __func__, _get_stack_ptr());
#ifdef CANCEL_ALARM
	static int i = 10;
	if (i-- <= 0) 
		CancelAlarm(1);
#endif
#ifdef EVENT_SET_CLEAR_TEST
	ClearEvent(1);
#endif
#ifdef GETEVENT_TEST

#ifdef WAITEVENT_TEST
	WaitEvent(2);
#endif
	EventMaskType Event = 0;
	ClearEvent(1);
	GetEvent(1, &Event);
	pr_log("Task B: Event = 0x%016X\n", Event);
#endif
}

TASK(Task_C) {
	pr_log("%s, sp=0x%08X\n", __func__, _get_stack_ptr());
}

void Alarm_uSecAlarm_callback(void) {
	pr_log("%s, sp=0x%08X\n", __func__, _get_stack_ptr());
}