#include <stdio.h>
#include <board.h>

#include <os_api.h>
#include <os_task.h>

#include <zephyr/kernel.h> // for k_cycle_get_32()
#include <zephyr/sys/time_units.h> // for k_cyc_to_us_near32()

#include <sg_counter.h>
#include <sg_alarms.h>
#include <sg_appmodes.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(os_alarm, LOG_LEVEL_DBG);


#define ENABLE_UPTIME_PRINTS	0 // set this to 1 if you want up-time to be printed.
#define TEMPORARY_WORKAROUND	1


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

	if (AlarmID >= MAX_OS_ALARMS) {
		LOG_ERR("Error: %s() invalid AlarmID %d", __func__, AlarmID);
		return E_OS_ID;
	}

	if (Info == NULL) {
		LOG_ERR("Error: %s() Info pointer is NULL", __func__);
		return E_OS_ARG_FAIL;
	}

	counter_id = _AlarmID2CounterID_map[AlarmID];
	if (counter_id >= OS_MAX_COUNTERS) {
		LOG_ERR("Error: %s(), Alarm to Counter mapping error!", __func__);
		return E_OS_STATE;
	}

	Info->mincycle = _OsCounterCtrlBlk[counter_id].mincycle;
	Info->ticksperbase = _OsCounterCtrlBlk[counter_id].ticksperbase;
	Info->maxallowedvalue = _OsCounterCtrlBlk[counter_id].maxallowedvalue;

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
	TickType ticks_curr, ticks_futr;
	const OsAlarmDataBlkType *data_blk;
	uint8_t blk_idx;

	if (AlarmID >= MAX_OS_ALARMS) {
		LOG_ERR("Error: %s() invalid AlarmID %d", __func__, AlarmID);
		return E_OS_ID;
	}

	if (Tick == NULL) {
		LOG_ERR("Error: %s() Tick pointer is NULL", __func__);
		return E_OS_ARG_FAIL;
	}

	counter_id = _AlarmID2CounterID_map[AlarmID];
	if (counter_id >= OS_MAX_COUNTERS) {
		LOG_ERR("Error: %s(), Alarm to Counter mapping error!", __func__);
		return E_OS_STATE;
	}

	blk_idx  = _AlarmID2BlkIndex_map[AlarmID];
	data_blk = _OsAlarmsGroups[counter_id].data_blk;
	ticks_futr = data_blk[blk_idx].counter;
	if (_OsCounterCtrlBlk[counter_id].maxallowedvalue < ONE_MSEC_IN_MICROSEC) {
		ticks_curr = k_cyc_to_us_near32(k_cycle_get_32());
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
	OsAlarmDataBlkType *data_blk;
	uint8_t blk_idx;

	if (AlarmID >= MAX_OS_ALARMS) {
		LOG_ERR("Error: %s() invalid AlarmID %d", __func__, AlarmID);
		return E_OS_ID;
	}

	counter_id = _AlarmID2CounterID_map[AlarmID];
	if (counter_id >= OS_MAX_COUNTERS) {
		LOG_ERR("Error: %s(), Alarm to Counter mapping error!", __func__);
		return E_OS_STATE;
	}

	if (increment > _OsCounterCtrlBlk[counter_id].maxallowedvalue) {
		LOG_ERR("Error: %s(), increment arg (=%d) > maxallowedvalue!",
			__func__, increment);
		return E_OS_LIMIT;
	}

	if (cycle < _OsCounterCtrlBlk[counter_id].mincycle) {
		LOG_ERR("Error: %s(), cycle arg (=%d) < mincycle!", __func__,
			cycle);
		return E_OS_LIMIT;
	}

	/* All inputs are validated, just configure alarms */
	blk_idx  = _AlarmID2BlkIndex_map[AlarmID];
	data_blk = (OsAlarmDataBlkType *) _OsAlarmsGroups[counter_id].data_blk; // data_blk points to RAM
	data_blk[blk_idx].counter = (TickType)(_GetOsTickCnt() + increment);
	data_blk[blk_idx].cycle = cycle;
	data_blk[blk_idx].is_active = TRUE;

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
	OsAlarmDataBlkType *data_blk;
	uint8_t blk_idx;

	if (AlarmID >= MAX_OS_ALARMS) {
		LOG_ERR("Error: %s() invalid AlarmID %d", __func__, AlarmID);
		return E_OS_ID;
	}

	counter_id = _AlarmID2CounterID_map[AlarmID];
	if (counter_id >= OS_MAX_COUNTERS) {
		LOG_ERR("Error: %s(), Alarm to Counter mapping error!", __func__);
		return E_OS_STATE;
	}

	if (start > _OsCounterCtrlBlk[counter_id].maxallowedvalue) {
		LOG_ERR("Error: %s(), start arg (=%d) > maxallowedvalue!",
			__func__, start);
		return E_OS_LIMIT;
	}

	if (cycle < _OsCounterCtrlBlk[counter_id].mincycle) {
		LOG_ERR("Error: %s(), cycle arg (=%d) < mincycle!", __func__,
			cycle);
		return E_OS_LIMIT;
	}

	/* All inputs are validated, just configure alarms */
	blk_idx  = _AlarmID2BlkIndex_map[AlarmID];
	data_blk = (OsAlarmDataBlkType *) _OsAlarmsGroups[counter_id].data_blk; // points to RAM
	data_blk[blk_idx].counter = start;
	data_blk[blk_idx].cycle = cycle;
	data_blk[blk_idx].is_active = TRUE;
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
	OsAlarmDataBlkType *data_blk;
	AlarmType counter_id;
	uint8_t blk_idx;

	if (AlarmID >= MAX_OS_ALARMS) {
		LOG_ERR("Error: %s() invalid AlarmID %d", __func__, AlarmID);
		return E_OS_ID;
	}

	counter_id = _AlarmID2CounterID_map[AlarmID];
	if (counter_id >= OS_MAX_COUNTERS) {
		LOG_ERR("Error: %s(), Alarm to Counter mapping error!", __func__);
		return E_OS_STATE;
	}

	/* All inputs are validated, just configure alarms */
	blk_idx  = _AlarmID2BlkIndex_map[AlarmID];
	data_blk = (OsAlarmDataBlkType *) _OsAlarmsGroups[counter_id].data_blk; // points to RAM.
	data_blk[blk_idx].is_active = FALSE;
	data_blk[blk_idx].counter = 0;
	data_blk[blk_idx].cycle = 0;
	return E_OK;
}


///////////////////////////////////////////////////////////////////////////////
// Core OS Alarm Functions
/*/ 
 Function: OsTriggerAlarm
 Description: This is called by OsHandleAlarms(). This function handles the
 alarm action as per the configuration done in OSEK builder file.
/*/
int OsTriggerAlarm(const OsAlarmCtrlBlkType* alarm) {
	int retval = 0;

	if (alarm == NULL) {
		LOG_ERR("Error: %s() invalid argument!", __func__);
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
		LOG_ERR("Error: Invalid Alarm Action Type in %s()", __func__);
		retval = -1;
		break;
	};

	return retval;
}


/// @brief This is called by OsHandleCounters(). This function parses all 
/// alarms that uses cntr_id as its base and check if the timer expired. If expires
/// this function triggers the Alarm actions.
/// @param cntr_id OSEK Counter ID
/// @param cnt Configured counter free running count value
/// @return 0 if all went good.
int OsHandleAlarms(int cntr_id, TickType cnt) {
	OsAlarmDataBlkType *data_blk;
	const OsAlarmCtrlBlkType *ctrl_blk;
	int i;

	if (cntr_id >= MAX_ALARM_COUNTERS) {
		LOG_ERR("Error: Counter ID %d is invalid in %s()", cntr_id, __func__);
		return -1;
	}

	/* loop through all configured alarms for counter 'cntr_id' */
	for (i = 0; i < _OsAlarmsGroups[cntr_id].len; i++) {
		/* get the alarm RAM area in a pointer */
		data_blk = (OsAlarmDataBlkType *) _OsAlarmsGroups[cntr_id].data_blk;

		/* if alarm is in disabled state, ignore it */
		if (!data_blk[i].is_active) {
			continue;
		}

		/* check if the counter's free running count value exceeded the alarm time */
		if (cnt >= data_blk[i].counter) {
			/* raise the OSEK alarm */
			ctrl_blk = _OsAlarmsGroups[cntr_id].ctrl_blk;
			OsTriggerAlarm(&ctrl_blk[i]);

			/* if cycletime is set, go for rounds in cycle */
			if (data_blk[i].cycle > 0) {
				data_blk[i].counter = cnt + data_blk[i].cycle;

				/* do adjustment for counter max saw-tooth peak */
				if (data_blk[i].counter > _OsCounterCtrlBlk[cntr_id].maxallowedvalue) {
					data_blk[i].counter -= _OsCounterCtrlBlk[cntr_id].maxallowedvalue;
				}
			}
			else {
				/* this one is a one-shot alarm, disable it */
				data_blk[i].is_active = FALSE;
			}
		}
	}

	return 0;
}
