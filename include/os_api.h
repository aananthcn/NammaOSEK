#ifndef AANANTH_OSEK_OS_API_H
#define AANANTH_OSEK_OS_API_H

#include <ostypes.h>

u32 _GetOsTickCnt(void);
int _OsHandleTicks(void);
u32 _switch_stack(u32 new_stack, u32 old_stack);

int pr_log(const char *format, ...);
int pr_log_init(void);


#endif