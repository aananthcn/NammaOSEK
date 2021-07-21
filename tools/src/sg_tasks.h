#ifndef ACN_OSEK_SG_TASKS_H
#define ACN_OSEK_SG_TASKS_H

#include <osek.h>


enum eAppModeType {
	OSDEFAULTAPPMODE,
	MANUFACT_MODE,
	HW_TEST_MODE,
	OS_MODES_MAX
};


typedef void (*TaskFuncType)(void);

typedef struct {
    TaskType id;
    TaskFuncType handler;
    u32 priority;
    u8 sch_type;
    bool autostart;
} OsTaskType;

extern OsTaskType OsTaskList[];



#endif
