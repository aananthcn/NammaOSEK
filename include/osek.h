#ifndef KERNEL_OSEK_H
#define KERNEL_OSEK_H

#include <ostypes.h>

/*
 * OS Status
 */
typedef enum eStatusType {
	E_OK = 0,
	E_OS_ACCESS = 1,
	E_OS_CALLEVEL = 2,
	E_OS_ID = 3,
	E_OS_LIMIT = 4,
	E_OS_NOFUNC = 5,
	E_OS_RESOURCE = 6,
	E_OS_STATE = 7,
	E_OS_VALUE = 8,
	E_OS_ARG_FAIL = 100,
	E_OS_STATUS_TYPE_MAX
} StatusType;


/*
 * Task States
 */
typedef enum eTaskStateType {
	INVALID_UNINITIALIZED,
	SUSPENDED,
	READY,
	RUNNING,
	WAITING,
	MAX_TASK_STATE
} TaskStateType;
typedef TaskStateType* TaskStateRefType;

/* 
 * The TaskType is forced to 32 bit to gain code efficiency on both 32bit and 
 * 64bit machines 
 */
typedef u32 TaskType;
typedef TaskType* TaskRefType;

/*
 * Scheduling types
 */
typedef enum eSchTypes {
	NON_PREEMPTIVE,
	FULL_PREEMPTIVE
} SchType;

/*
 * Task Declarations & Definitions
 */
#define DeclareTask(TaskName) void OSEK_Task_##TaskName(void)
#define TASK(TaskName) void OSEK_Task_##TaskName(void)


/* 
 * This defintion is made as per SWS Item "ECUC_Os_00022' defined in AUTOSAR OS
 * SWS, which says "An OsAppMode called OSDEFAULTAPPMODE must always be there for
 *ISO 17356 compatibility." 
 */
#if 0
typedef enum eAppModeType {
	OSDEFAULTAPPMODE, /* Default application mode, always a valid parameter to StartOS */
	OS_SLEEP_MODE,
	OS_ERROR_MODE,
	OS_MODES_MAX
} AppModeType;
#endif
typedef u32 AppModeType;

// function prototypes
int SetActiveApplicationMode(AppModeType mode);
AppModeType GetActiveApplicationMode(void);
void StartOS(AppModeType mode);
void ShutdownOS(StatusType Error);


/* 
 * System Services
 * Conformance: BCC1, BCC2, ECC1, ECC2
 */
StatusType ActivateTask(TaskType TaskID);
StatusType TerminateTask(void);
StatusType ChainTask(TaskType TaskID);
StatusType Schedule(void);
StatusType GetTaskID(TaskRefType TaskID);
StatusType GetTaskState(TaskType TaskID, TaskStateRefType State);


/*
 * Interrupt Services & Macros
 */
void EnableAllInterrupts(void);
void DisableAllInterrupts(void);
void ResumeAllInterrupts(void);
void SuspendAllInterrupts(void);
void ResumeOSInterrupts(void);
void SuspendOSInterrupts(void);
#define ISR(FuncName)) void OSEK_ISR_##FuncName)(void)

/*
 * Resource Management - At the moment (26-Jun-2021) I am assuming two types of resources
 * 1) Memory
 * 2) Device (memory mapped)
 * 3) Scheduler
 */
typedef enum eResType {
	RES_MEMORY,
	RES_DEVICE,
	RES_SCHEDULER,
	RES_MAX
} ResType;

typedef struct sResObjType {
	ResType type;
	bool in_use;
	void* address;
} ResObjType;

/* The values of ResourceType starts from 0 to 4 million */
typedef u32 ResourceType;

/* 
 * The plan is to place resources into a linker section so that we can count
 * the number of resources to give IDs, even if the definitions are distributed
 * across files.
 */
#define DeclareResource(R)	extern ResObjType* R __attribute__((section("osek_resource")));

StatusType GetResource(ResourceType ResID);
StatusType ReleaseResource(ResourceType ResID);


/* 
 * OS Events
 */

/* Data type of the event mask. */
typedef u64 EventMaskType;
typedef EventMaskType* EventMaskRefType;

/* 
 * The plan is to place all events into a linker section so that we can count
 * the number of events to give IDs, even if the definitions are distributed
 * across files.
 */
#define DeclareEvent(E)	extern EventMaskType* E __attribute__((section("osek_event")));
StatusType SetEvent(TaskType TaskID, EventMaskType Mask);
StatusType ClearEvent(EventMaskType Mask);
StatusType GetEvent(TaskType TaskID, EventMaskRefType Event);
StatusType WaitEvent(EventMaskType Mask);


/*
 * Alarms
 */

/* This data type represents count values in ticks.*/
typedef u32 TickType;
typedef TickType* TickRefType;

/* This data type represents a structure for storage of counter characteristics */
typedef struct sAlarmBaseType {
	/* Maximum possible allowed count value in ticks */
	TickType maxallowedvalue; 

	/* Number of ticks required to reach a counter-specific (significant) unit. */
	TickType ticksperbase;

	/* Smallest allowed value for the cycle-parameter of SetRelAlarm/SetAbsAlarm) 
	(only for systems with extended status). */
	TickType mincycle;
} AlarmBaseType;
typedef AlarmBaseType* AlarmBaseRefType;

/* This data type represents an alarm object. */
typedef u32 AlarmType;

/* 
 * The plan is to place all Alarms into a linker section so that we can count
 * the number of alarms to give IDs, even if the definitions are distributed
 * across files.
 */
#define DeclareAlarm(A)	extern AlarmType* A __attribute__((section("osek_event")));

StatusType GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info);
StatusType GetAlarm(AlarmType AlarmID, TickRefType Tick);
StatusType SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle);
StatusType SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle);
StatusType CancelAlarm(AlarmType AlarmID);

/* Maximum possible allowed value of the system counter in ticks. */
#define OSMAXALLOWEDVALUE	(0xFFFFFFFF)

/* Number of ticks required to reach a specific unit of the system counter. */
#define OSTICKSPERBASE		(1)

/* Minimum allowed number of ticks for a cyclic alarm of the system counter. */
#define OSMINCYCLE		(1)

/* Duration of a tick of the system counter in nanoseconds. */
#define OSTICKDURATION		(1)

#define ALARMCALLBACK(AlarmCallBackName))) void OSEK_Alarm_##AlarmCallBackName))(void)



/*
 * HOOK Routines
 */
//typedef u32 OSServiceIdType;

void ErrorHook (StatusType Error);
void PreTaskHook(void);
void PostTaskHook(void);
void StartupHook(void);
void ShutdownHook(StatusType Error);


/*
 * Service Identifiers
 */
typedef enum eOSServiceIdType {
	OSServiceId_ActivateTask,
	OSServiceId_TerminateTask,
	OSServiceId_ChainTask,
	OSServiceId_Schedule,
	OSServiceId_GetTaskID,
	OSServiceId_GetTaskState,
	OSServiceId_GetResource,
	OSServiceId_ReleaseResource,
	OSServiceId_SetEvent,
	OSServiceId_ClearEvent,
	OSServiceId_GetEvent,
	OSServiceId_WaitEvent,
	OSServiceId_GetAlarmBase,
	OSServiceId_GetAlarm,
	OSServiceId_SetRelAlarm,
	OSServiceId_SetAbsAlarm,
	OSServiceId_CancelAlarm,
	OSServiceId_MAX
} OSServiceIdType;

#endif