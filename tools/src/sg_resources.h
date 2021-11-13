#ifndef ACN_OSEK_SG_RESOURCES_H
#define ACN_OSEK_SG_RESOURCES_H

#include <osek.h>


#define RES(x)  RES_##x

typedef enum {
	RES_mutex1,
	MAX_RESOURCE_ID
} OsResourcesType;


typedef struct {
    ResourceType* res;
    u16 ceil_prio;
    u16 n_tasks;
    const TaskType* task_ids;
} OsResMapType;

extern const OsResMapType _OsResList[];


#endif
