#include "sg_counter.h"


OsCounterType _OsCounters[] =  {
	{
		.alarm.mincycle = 1,
		.alarm.maxallowedvalue = 0xFFFFFFFF,
		.alarm.ticksperbase = 1,
		.tickduration = 1000000,
		.name = "mSecCounter"
	},
	{
		.alarm.mincycle = 100,
		.alarm.maxallowedvalue = 0xFFFFFFFF,
		.alarm.ticksperbase = 1,
		.tickduration = 1000,
		.name = "uSecCounter"
	}
};
