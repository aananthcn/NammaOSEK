#ifndef ACN_OSEK_SG_COUNTER_H
#define ACN_OSEK_SG_COUNTER_H
#include <osek.h>


#define OS_MAX_COUNTERS    (2)

typedef struct {
	char* name;
	u32 mincycle;
	u32 maxallowedvalue;
	u32 ticksperbase;
	u32 tickduration;
} OsCounterType;



#endif
