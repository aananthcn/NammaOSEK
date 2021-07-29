#ifndef ACN_OSEK_SG_TASKS_H
#define ACN_OSEK_SG_TASKS_H

#include <osek.h>
#include <osek_com.h>


enum eTaskType {
	TASK_TASK_A_ID,
	TASK_TASK_B_ID,
	TASK_TASK_C_ID,
	TASK_ID_MAX
};


#define TASK_A_APPMODE_MAX	(3)
#define TASK_A_RESOURCE_MAX	(0)
#define TASK_A_EVENT_MAX	(0)
#define TASK_A_MESSAGE_MAX	(1)

#define TASK_B_APPMODE_MAX	(0)
#define TASK_B_RESOURCE_MAX	(1)
#define TASK_B_EVENT_MAX	(1)
#define TASK_B_MESSAGE_MAX	(1)

#define TASK_C_APPMODE_MAX	(1)
#define TASK_C_RESOURCE_MAX	(0)
#define TASK_C_EVENT_MAX	(2)
#define TASK_C_MESSAGE_MAX	(0)



typedef void (*TaskFuncType)(void);

typedef struct {
    TaskType id;
    TaskFuncType handler;
    u32 priority;
    u8 sch_type;
    u32 activation;
    bool autostart;
    const AppModeType** app_modes;
    u32 n_app_modes;
    MessageType** msg;
    u32 n_msg;
    ResourceType** res;
    u32 n_res;
    const EventMaskType** evt_msk;
    u32 n_evt;
} OsTaskType;

extern const OsTaskType OsTaskList[];



#endif
