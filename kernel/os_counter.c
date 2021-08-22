#include <stdio.h>
#include <board.h>

#include <sg_counter.h>
#include <sg_alarms.h>
#include <os_api.h>
#include <os_task.h>

static u32 OsTickCount;
static u32 OsTickCount_us;

#define ENABLE_UPTIME_PRINTS	1
#define TEMPORARY_WORKAROUND	1
#define ONE_MSEC_IN_NANO_SEC	(1000000)


int OsComputeUpTime(void);
int OsHandleCounters(void);


u32 GetOsTickCnt(void) {
	return OsTickCount;
}


int OsHandleTicks(void) {
	OsTickCount++;
	ClearActivationsCounts();

	// TODO: Set some event for OS to process the OS Ticks
#ifdef TEMPORARY_WORKAROUND
	// The following line below is a temporary work-around. This is bad
	// because we are processing counter within ISR. This should be avoided.
	OsHandleCounters();
#endif

#ifdef ENABLE_UPTIME_PRINTS
	OsComputeUpTime();
#endif
	return 0;
}

int OsHandleAlarms(int cntr_id) {
	int i;

	if (cntr_id >= MAX_APP_ALARMS) {
		pr_log("Error: Counter ID %d is invalid in %s()\n", cntr_id, __func__);
		return -1;
	}

	for (i = 0; i < AppAlarms[cntr_id].len; i++) {
		pr_log("Name = %s\n", AppAlarms[cntr_id].alarm[i].name);
	}
}


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
	os_ticks = GetOsTickCnt();

	/* Increment user configured OSEK Counters */
	for (int i = 0; i < OS_MAX_COUNTERS; i++) {
		if (OsCounters[i].tickduration < ONE_MSEC_IN_NANO_SEC ) {
			delta = (TickType)(nsec_cnt - nsec_cnt_old);
		}
		else {
			delta = (TickType)(os_ticks - os_ticks_old);
		}

		if (delta >= OsCounters[i].alarm.ticksperbase) {
			OsCounters[i].countval += delta;
			if (OsCounters[i].countval > OsCounters[i].alarm.maxallowedvalue) {
				OsCounters[i].countval = 0;
			}
		}
		OsHandleAlarms(i);
	}

	os_ticks_old = os_ticks;
	nsec_cnt_old = nsec_cnt;
	return 0;
}


#ifdef ENABLE_UPTIME_PRINTS
int OsComputeUpTime(void) {
	static u64 upTime_sec, ut_old;
	u64 sec_in_nano_sec = 1000000000; /* 1 sec = 1000,000,000 nano sec */
	int days, hrs, min, sec;

	/* Print all counters */
	printf("[");
	for (int i = 0; i < OS_MAX_COUNTERS; i++) {
		printf("%08X", OsCounters[i].countval);
		if (i+1 != OS_MAX_COUNTERS)
			printf(", ");
	}
	printf("] ");

	/* Compute and print up-time */
	upTime_sec = (u64) OS_TICK_DURATION_ns * GetOsTickCnt() / sec_in_nano_sec;
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