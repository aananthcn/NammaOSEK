#ifndef KERNEL_OSEK_H
#define KERNEL_OSEK_H

/* 
 * This defintion is made as per SWS Item "ECUC_Os_00022' defined in AUTOSAR OS
 * SWS, which says "An OsAppMode called OSDEFAULTAPPMODE must always be there for
 *ISO 17356 compatibility." 
 */
typedef enum OS_MODES {
	OSDEFAULTAPPMODE,
	OS_SHUTDOWN,
	OS_ERROR_MODE,
	OS_MODES_MAX
} os_modes_t;

// function prototypes
int SetOsMode(os_modes_t mode);
os_modes_t GetOsMode(void);
void StartOS(os_modes_t mode);

#endif