#ifndef AANANTH_OSEK_OS_API_H
#define AANANTH_OSEK_OS_API_H

#include <ostypes.h>

u32 GetOsTickCnt(void);
int OsHandleTicks(void);

int pr_log(const char *format, ...);
int pr_log_init(void);


#endif