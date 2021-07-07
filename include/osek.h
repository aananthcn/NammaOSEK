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
	E_OS_STATUS_TYPE_MAX
} StatusType;


/*
 * Task States
 */
typedef enum eTaskStateType {
	SUSPENDED,
	READY,
	RUNNING,
	WAITING,
	INVALID_TASK
} TaskStateType;
typedef TaskStateType* TaskStateRefType;

/* 
 * The TaskType is forced to 32 bit to gain code efficiency on both 32bit and 
 * 64bit machines 
 */
typedef u32 TaskType;
typedef u32* TaskRefType;

/*
 * Task Declarations & Definitions
 */
#define DeclareTask(TaskName)) void OSEK_Task_##TaskName)(void)
#define TASK(TaskName)) void OSEK_Task_##TaskName)(void)


/* 
 * This defintion is made as per SWS Item "ECUC_Os_00022' defined in AUTOSAR OS
 * SWS, which says "An OsAppMode called OSDEFAULTAPPMODE must always be there for
 *ISO 17356 compatibility." 
 */
typedef enum eOsModeType {
	OSDEFAULTAPPMODE,
	OS_SHUTDOWN_MODE,
	OS_ERROR_MODE,
	OS_MODES_MAX
} OsModeType;

// function prototypes
int SetOsMode(OsModeType mode);
OsModeType GetOsMode(void);
void StartOS(OsModeType mode);


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
void ResumeAllInterrupts ( void );
void SuspendAllInterrupts ( void );
void ResumeOSInterrupts ( void );
void SuspendOSInterrupts ( void );
#define ISR(FuncName)) void OSEK_ISR_##FuncName)(void)

/*
 * Resource Management - At the moment I am assuming two types of resources
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
#define DeclareResource(R)	extern ResObjType* R __attribute__((section("osek_res")));

StatusType GetResource(ResourceType ResID);
StatusType ReleaseResource(ResourceType ResID);


#endif