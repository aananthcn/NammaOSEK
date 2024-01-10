#ifndef AANANTH_OSEK_OS_API_H
#define AANANTH_OSEK_OS_API_H

#include <base_types.h>
#include <stdio.h>



// Macros
#define ONE_MSEC_IN_MICROSEC	(1000)


// Public functions
u32 _GetOsTickCnt(void);
int _OsHandleTicks(void);


// logging functions
#define pr_log(...) 	do {} while(0)


#endif