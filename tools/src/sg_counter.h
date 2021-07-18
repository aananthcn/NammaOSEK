#ifndef ACN_OSEK_SG_COUNTER_H
#define ACN_OSEK_SG_COUNTER_H
#include <osek.h>


typedef struct {
	char* name;
	u32 mincycle;
	u32 maxallowedvalue;
	u32 ticksperbase;
	u32 tickduration;
} OsCounterType;

OsCounterType OsCounters[];

#define OS_TICK_DURATION_ns 	(1000000)
#define OS_TICK_COUNTER_IDX 	(0)
#define OS_MAX_COUNTERS    	(2)


#endif
