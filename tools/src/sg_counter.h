#ifndef ACN_OSEK_SG_COUNTER_H
#define ACN_OSEK_SG_COUNTER_H
#include <osek.h>


typedef struct {
	u32 countval; /* continuos incrementing counter */ 
	u32 maxallowedvalue; /* upper limit, after which counter resets to 0 */
	u32 tickduration; /* count in nano seconds */
	u16 ticksperbase; /* no of ticks to increment a count */ 
	u16 mincycle; /* minimum value allowed by user to set alarms */ 
	char* name;
} OsCounterType;

extern OsCounterType OsCounters[];

#define OS_TICK_DURATION_ns 	(1000000)
#define OS_TICK_COUNTER_IDX 	(0)
#define OS_MAX_COUNTERS    	(2)


#endif
