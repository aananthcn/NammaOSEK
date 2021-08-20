#ifndef ACN_OSEK_SG_ALARMS_H
#define ACN_OSEK_SG_ALARMS_H
#include <osek.h>


typedef enum {
    AAT_ACTIVATETASK,
    AAT_SETEVENT,
    AAT_ALARMCALLBACK,
    AAT_MAX_TYPE
} AlarmActionType;


typedef struct {
    char* name;                     /* short name of alarm */ 
    AlarmType cntr_id;              /* OS Counter ID (= index of OsCounters + 1) */ 
    AlarmActionType aat;            /* Refer enum AlarmActionType */ 
    void* aat_arg1;                 /* arg1: task_name | callback_fun */
    void* aat_arg2;                 /* arg2: event | NULL */
    bool is_autostart;              /* does this alarm start at startup? */
    u32 alarmtime;                  /* when does it expire? */
    u32 cycletime;                  /* cyclic time - for repetition */
    const AppModeType** appmodes;   /* alarm active in which modes? */
    u32 n_appmodes;                 /* how may appmodes for this entry? */
} AppAlarmType;

extern AppAlarmType AppAlarms[];
extern const AppModeType Alarm_WakeTaskA_AppModes[];


#endif
