#include <stdio.h>
#include <board.h>

#include <os_api.h>
#include <os_task.h>

#include <sg_counter.h>
#include <sg_alarms.h>
#include <sg_appmodes.h>

static u32 _OsTickCount;
static u32 OsTickCount_us;

#define ENABLE_UPTIME_PRINTS	0 // set this to 1 if you want up-time to be printed.
#define TEMPORARY_WORKAROUND	1
#define ONE_MSEC_IN_NANO_SEC	(1000000)


int OsComputeUpTime(void);



///////////////////////////////////////////////////////////////////////////////
// OSEK Functions
/*/
Function: GetAlarmBase

Description: The system service GetAlarmBase reads the alarm base characteristics. 
The return value <Info> is a structure in which the information of data type 
AlarmBaseType is stored.

Particularities: Allowed on task level, ISR, and in several hook routines.
/*/
StatusType GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info) {
	AlarmType counter_id;

	if (AlarmID >= MAX_APP_ALARM_COUNTERS) {
		pr_log("Error: %s() invalid AlarmID %d\n", __func__, AlarmID);
		return E_OS_ID;
	}

	if (Info == NULL) {
		pr_log("Error: %s() Info pointer is NULL\n", __func__);
		return E_OS_ARG_FAIL;
	}

	counter_id = _AlarmID2CounterID_map[AlarmID];
	if (counter_id >= OS_MAX_COUNTERS) {
		pr_log("Error: %s(), Alarm to Counter mapping error!\n", __func__);
		return E_OS_STATE;
	}

	*Info = _OsCounters[counter_id].alarm;
	return E_OK;
}

/*/
Function: GetAlarm

Description: The system service GetAlarm returns the relative value in ticks 
before the alarm <AlarmID> expires.

Particularities: It is up to the application to decide whether for example a 
CancelAlarm may still be useful. If <AlarmID> is not in use, <Tick> is not 
defined. Allowed on task level, ISR, and in several hook routines
/*/
StatusType GetAlarm(AlarmType AlarmID, TickRefType Tick) {
	AlarmType counter_id;
	TickType ticks_curr, ticks_futr, delta;

	if (AlarmID >= MAX_APP_ALARM_COUNTERS) {
		pr_log("Error: %s() invalid AlarmID %d\n", __func__, AlarmID);
		return E_OS_ID;
	}

	if (Tick == NULL) {
		pr_log("Error: %s() Tick pointer is NULL\n", __func__);
		return E_OS_ARG_FAIL;
	}

	counter_id = _AlarmID2CounterID_map[AlarmID];
	if (counter_id >= OS_MAX_COUNTERS) {
		pr_log("Error: %s(), Alarm to Counter mapping error!\n", __func__);
		return E_OS_STATE;
	}

	ticks_futr = _AppAlarmCounters[AlarmID];
	if (_OsCounters[counter_id].maxallowedvalue < ONE_MSEC_IN_NANO_SEC) {
		if (brd_get_usec_syscount(&ticks_curr)) {
			pr_log("Error: brd_get_usec_syscount returns error\n");
			return -1;
		}
		*Tick = (TickType)(ticks_futr - ticks_curr);
	}
	else {
		ticks_curr = _GetOsTickCnt();
		*Tick = (TickType)(ticks_futr - ticks_curr);
	}

	return E_OK;
}

/*/
Function: SetRelAlarm

Description: The system service occupies the alarm <AlarmID> element. After 
<increment> ticks have elapsed, the task assigned to the alarm <AlarmID> is 
activated or the assigned event (only for extended tasks) is set or the 
alarm-callback routine is called.

Particularities: The behaviour of <increment> equal to 0 is up to the 
implementation.
If the relative value <increment> is very small, the alarm may expire, and the
task may become ready or the alarm-callback may be called before the system 
service returns to the user.
If <cycle> is unequal zero, the alarm element is logged on again immediately 
after expiry with the relative value <cycle>.
The alarm <AlarmID> must not already be in use.
To change values of alarms already in use the alarm shall be cancelled first.
If the alarm is already in use, this call will be ignored and the error 
E_OS_STATE is returned.
Allowed on task level and in ISR, but not in hook routines.
/*/
StatusType SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle) {
	AlarmType counter_id;

	if (AlarmID >= MAX_APP_ALARM_COUNTERS) {
		pr_log("Error: %s() invalid AlarmID %d\n", __func__, AlarmID);
		return E_OS_ID;
	}

	counter_id = _AlarmID2CounterID_map[AlarmID];
	if (counter_id >= OS_MAX_COUNTERS) {
		pr_log("Error: %s(), Alarm to Counter mapping error!\n", __func__);
		return E_OS_STATE;
	}

	if (increment > _OsCounters[counter_id].alarm.maxallowedvalue) {
		pr_log("Error: %s(), increment arg (=%d) > maxallowedvalue!\n",
			__func__, increment);
		return E_OS_LIMIT;
	}

	if (cycle < _OsCounters[counter_id].alarm.mincycle) {
		pr_log("Error: %s(), cycle arg (=%d) < mincycle!\n", __func__,
			cycle);
		return E_OS_LIMIT;
	}

	/* All inputs are validated, just configure alarms */
	_AppAlarmCounters[AlarmID] = (TickType)(_OsTickCount + increment);
	_AppAlarmCycles[AlarmID] = cycle;
	_AppAlarmStates[AlarmID] = true;
	return E_OK;
}

/*/
Function: SetAbsAlarm

Parameters:
  AlarmID  Reference to the alarm element
  start    Absolute value in ticks
  cycle    Cycle value in case of cyclic alarm. In case of single alarms,
           cycle shall be zero.

Description: The system service occupies the alarm <AlarmID> element. 
When <start> ticks are reached, the task assigned to the alarm <AlarmID> 
is activated or the assigned event (only for extended tasks) is set or 
the alarm-callback routine is called.
/*/
StatusType SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle) {
	AlarmType counter_id;

	if (AlarmID >= MAX_APP_ALARM_COUNTERS) {
		pr_log("Error: %s() invalid AlarmID %d\n", __func__, AlarmID);
		return E_OS_ID;
	}

	counter_id = _AlarmID2CounterID_map[AlarmID];
	if (counter_id >= OS_MAX_COUNTERS) {
		pr_log("Error: %s(), Alarm to Counter mapping error!\n", __func__);
		return E_OS_STATE;
	}

	if (start > _OsCounters[counter_id].alarm.maxallowedvalue) {
		pr_log("Error: %s(), start arg (=%d) > maxallowedvalue!\n",
			__func__, start);
		return E_OS_LIMIT;
	}

	if (cycle < _OsCounters[counter_id].alarm.mincycle) {
		pr_log("Error: %s(), cycle arg (=%d) < mincycle!\n", __func__,
			cycle);
		return E_OS_LIMIT;
	}

	/* All inputs are validated, just configure alarms */
	_AppAlarmCounters[AlarmID] = start;
	_AppAlarmCycles[AlarmID] = cycle;
	_AppAlarmStates[AlarmID] = true;
	return E_OK;
}


/*/
Function: CancelAlarm

Parameters:
  AlarmID  Reference to the alarm element

Description: The system service cancels the alarm <AlarmID>.
Particularities: Allowed on task level and in ISR, but not in hook routines.
/*/
StatusType CancelAlarm(AlarmType AlarmID) {
	if (AlarmID >= MAX_APP_ALARM_COUNTERS) {
		pr_log("Error: %s() invalid AlarmID %d\n", __func__, AlarmID);
		return E_OS_ID;
	}

	/* All inputs are validated, just configure alarms */
	_AppAlarmStates[AlarmID] = false;
	_AppAlarmCounters[AlarmID] = 0;
	_AppAlarmCycles[AlarmID] = 0;
	return E_OK;
}


///////////////////////////////////////////////////////////////////////////////
// Core OS Alarm / Counter Functions
u32 _GetOsTickCnt(void) {
	return _OsTickCount;
}


/*/ 
 Function: _OsHandleTicks
 Description: This is called by brd_setup_sytimer() up on every basic OS ticks 
 which in-turned is dervived from OS_TICK_DURATION_ns macro generated by OSEK 
 builder & System Generator tools. 
/*/
int _OsHandleTicks(void) {
	_OsTickCount++;
	OsClearActivationsCounts();

#if ENABLE_UPTIME_PRINTS != 0
	OsComputeUpTime();
#endif
	return 0;
}


int OsInitializeAlarms(AppModeType mode) {
	int ctr, alm, m;
	bool is_mode_ok = false;
	const AppAlarmType* alarm;

	if (mode >= OS_MODES_MAX) {
		pr_log("Error: AppMode \"%d >= OS_MODES_MAX\". Alarm init failed!\n", mode);
		return -1;
	}

	for (ctr = 0; ctr < MAX_APP_ALARMS; ctr++) {
		for (alm = 0; alm < _AppAlarms[ctr].len; alm++) {
			alarm = &_AppAlarms[ctr].alarm[alm];
			if (alarm->is_autostart != true) {
				continue;
			}
			for (m = 0; m < alarm->n_appmodes; m++) {
				if (alarm->appmodes[m] == mode) {
					is_mode_ok = true;
					break;
				}
			}
			if (!is_mode_ok) {
				continue;
			}

			/* reaching here means, we need to trigger alarms */
			*alarm->pacntr = alarm->alarmtime;
			*alarm->pcycle = alarm->cycletime;
			*alarm->palrm_state = true;
		}
	}

	pr_log("OSEK Alarms intialization done!\n");
	return 0;
}


/*/ 
 Function: OsTriggerAlarm
 Description: This is called by OsHandleAlarms(). This function handles the
 alarm action as per the configuration done in OSEK builder file.
/*/
int OsTriggerAlarm(const AppAlarmType* alarm) {
	int retval = 0;

	if (alarm == NULL) {
		pr_log("Error: %s() invalid argument!\n", __func__);
		return -1;
	}

	switch (alarm->aat) {
	case AAT_ACTIVATETASK:
		ActivateTask((TaskType)(alarm->aat_arg1));
		break;
	case AAT_SETEVENT:
		SetEvent((TaskType)alarm->aat_arg1, (EventMaskType)alarm->aat_arg2);
		break;
	case AAT_ALARMCALLBACK:
		((void (*)(void))alarm->aat_arg1)();
		break;
	default:
		pr_log("Error: Invalid Alarm Action Type in %s()\n", __func__);
		retval = -1;
		break;
	};

	return retval;
}


/*/ 
 Function: OsHandleAlarms
 Description: This is called by OsHandleCounters(). This function parses all 
 alarms that uses cntr_id as its base and check if the timer expired. If expires
 this function triggers the Alarm actions.
/*/
int OsHandleAlarms(int cntr_id, TickType cnt) {
	int i;
	const AppAlarmType* alarm;


	if (cntr_id >= MAX_APP_ALARMS) {
		pr_log("Error: Counter ID %d is invalid in %s()\n", cntr_id, __func__);
		return -1;
	}

	for (i = 0; i < _AppAlarms[cntr_id].len; i++) {
		alarm = &_AppAlarms[cntr_id].alarm[i];
		if ((cnt >= *alarm->pacntr) && (*alarm->palrm_state)) {
			OsTriggerAlarm(alarm);
			/* if cycletime is set, go for next round */
			if (*alarm->pcycle > 0) {
				*alarm->pacntr = cnt + *alarm->pcycle;
			}
			else {
				*alarm->palrm_state = false;
			}
		}
	}
}


/*/ 
 Function: OsHandleCounters
 Description: This is called by _OsHandleTicks(). This function refreshes all 
 counters configured within this call and also handle alarms if any alarm
 expires.
/*/
int OsHandleCounters(void) {
	static TickType os_ticks_old, nsec_cnt_old;
	TickType os_ticks, nsec_cnt;
	TickType delta;
	int i;

	/* Get input from OS Counter */
	if (brd_get_usec_syscount(&nsec_cnt)) {
		pr_log("Error: brd_get_usec_syscount returns error\n");
		return -1;
	}
	os_ticks = _GetOsTickCnt();

	/* Increment user configured OSEK Counters */
	for (int i = 0; i < OS_MAX_COUNTERS; i++) {
		if (_OsCounters[i].maxallowedvalue < ONE_MSEC_IN_NANO_SEC ) {
			delta = (TickType)(nsec_cnt - nsec_cnt_old);
		}
		else {
			delta = (TickType)(os_ticks - os_ticks_old);
		}

		if (delta >= _OsCounters[i].alarm.ticksperbase) {
			_OsCounters[i].countval += delta;
			if (_OsCounters[i].countval > _OsCounters[i].alarm.maxallowedvalue) {
				_OsCounters[i].countval = 0;
			}
		}
		OsHandleAlarms(i, _OsCounters[i].countval);
	}

	os_ticks_old = os_ticks;
	nsec_cnt_old = nsec_cnt;
	return 0;
}


#if ENABLE_UPTIME_PRINTS != 0
int OsComputeUpTime(void) {
	static u64 upTime_sec, ut_old;
	u64 sec_in_nano_sec = 1000000000; /* 1 sec = 1000,000,000 nano sec */
	int days, hrs, min, sec;

	/* Print all counters */
	printf("[");
	for (int i = 0; i < OS_MAX_COUNTERS; i++) {
		printf("%08X", _OsCounters[i].countval);
		if (i+1 != OS_MAX_COUNTERS)
			printf(", ");
	}
	printf("] ");

	/* Compute and print up-time */
	upTime_sec = (u64) OS_TICK_DURATION_ns * _GetOsTickCnt() / sec_in_nano_sec;
	if (upTime_sec != ut_old) {
		sec = upTime_sec % 60;
		min = (upTime_sec / 60) % 60;
		hrs = (upTime_sec / 3600) % 24;
		days = (upTime_sec / (24*3600));
		printf("Up-Time: %d days - %02d:%02d:%02d", days, hrs, min, sec);
		ut_old = upTime_sec;
	}
	printf("\r");
	fflush(stdout);
}
#endif