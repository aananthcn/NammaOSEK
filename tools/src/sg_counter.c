#include "sg_counter.h"


OsCounterType OsCounters[] =  {
	{
		.name = "mSecCounter",
		.mincycle = 1,
		.maxallowedvalue = 0xFFFFFFFF,
		.ticksperbase = 1,
		.tickduration = 1000000
	},
	{
		.name = "uSecCounter",
		.mincycle = 100,
		.maxallowedvalue = 0xFFFFFFFF,
		.ticksperbase = 1,
		.tickduration = 1000
	}
};
