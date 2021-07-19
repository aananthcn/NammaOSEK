#include <stdio.h>
#include <sg_counter.h>
#include <board.h>

static u32 OsTickCount;
static u32 OsTickCount_us;

//#define ENABLE_UPTIME_PRINTS	1
#define ONE_MSEC_IN_NANO_SEC	(1000000)


int OsComputeUpTime(void);
int OsHandleCounters(void);


u32 GetOsTickCnt(void) {
	return OsTickCount;
}

int OsHandleTicks(void) {
	OsTickCount++;
	if (brd_get_usec_syscount(&OsTickCount_us))
		printf("Error: brd_get_usec_syscount returned non-zero\n");

	// TODO: Set some event for OS to process the OS Ticks

	// The following line below is a temporary work-around. This is bad
	// because we are processing counter within ISR. This should be avoided.
	OsHandleCounters();

#ifdef ENABLE_UPTIME_PRINTS
	OsComputeUpTime();
#endif
	return 0;
}

int OsHandleCounters(void) {
	int i;
	u32 new_tick;
	u32 delta;

	for (int i = 0; i < OS_MAX_COUNTERS; i++) {
		if (OsCounters[i].tickduration < ONE_MSEC_IN_NANO_SEC ) {
			if (brd_get_usec_syscount(&new_tick)) {
				printf("Error: brd_get_usec_syscount returns \
					error\n");
				return -1;
			}
		}
		else {
			//TBD
			new_tick = GetOsTickCnt();
		}
		delta = new_tick - OsCounters[i].countval;
		if (delta > OsCounters[i].ticksperbase) {
			OsCounters[i].countval += delta;
			if (OsCounters[i].countval > OsCounters[i].maxallowedvalue) {
				OsCounters[i].countval = (OsCounters[i].countval - 
					OsCounters[i].maxallowedvalue);
			} 
		}
		printf("%u\n", OsCounters[i].countval);
	}

	return 0;
}

#ifdef ENABLE_UPTIME_PRINTS
int OsComputeUpTime(void) {
	static u64 upTime_sec, ut_old;
	u64 sec_in_nano_sec = 1000000000; /* 1 sec = 1000,000,000 nano sec */
	int days, hrs, min, sec;

	upTime_sec = (u64) OS_TICK_DURATION_ns * GetOsTickCnt() / sec_in_nano_sec;
	if (upTime_sec != ut_old) {
		sec = upTime_sec % 60;
		min = (upTime_sec / 60) % 60;
		hrs = (upTime_sec / 3600) % 24;
		days = (upTime_sec / (24*3600));
		printf("Up-Time: %d days - %02d:%02d:%02d\r", days, hrs, min, sec);
		fflush(stdout);
		ut_old = upTime_sec;
	}	
}
#endif