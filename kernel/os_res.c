// This file implements the OSEK resource management requirements
#include <osek.h>
#include <os_api.h>
#include <os_task.h>

#include <sg_resources.h>



typedef struct {
	ResourceType id;
	bool in_use;
} ResCtrlType;


static ResCtrlType _OsResCtrlBlk[MAX_RESOURCE_ID];


/*/
Function: GetResource
Argument: Resource ID
Description: 
	This call serves to enter critical sections in the code that are
	assigned to the resource referenced by <ResID>. A critical section
	shall always be left using ReleaseResource.

Particularities:
	The OSEK priority ceiling protocol for resource management. Nested
	resource occupation is only allowed if the inner critical sections are
	completely executed within the surrounding critical section. Nested
	occupation of one and the same resource is also forbidden! It is not
	allowed to use services which are points of rescheduling for non
	preemptable tasks (TerminateTask, ChainTask, Schedule and WaitEvent) in
	critical sections.
/*/
StatusType GetResource(ResourceType ResID) {
	StatusType stat = E_OK;

	if (ResID >= MAX_RESOURCE_ID) {
		pr_log("Error: %s() called with invalid ResID %d\n", __func__, ResID);
		return E_OS_ID;
	}

	DisableAllInterrupts();
	if (_OsResCtrlBlk[ResID].in_use) {
		EnableAllInterrupts();
		pr_log("Error: %s() ResID %d is busy!\n", __func__, ResID);
		return E_OS_RESOURCE;
	}
	_OsResCtrlBlk[ResID].in_use = true;
	EnableAllInterrupts();

	/* Let us raise the priority of the calling task to ceiling priority */
	OsSetCeilingPrio(_OsResList[ResID].ceil_prio);
	/* After this point, the calling task will be scheduled at higher priority
	till ReleaseResource() is called by the calling task. This is done as per
	OSEK Priority Ceiling Protocol! */

	return stat;
}


/*/
Function: ReleaseResource
Argument: Resource ID
Description: 
	ReleaseResource is the counterpart of GetResource and serves to leave
	critical sections in the code that are assigned to the resource
	referenced by <ResID>.

Particularities:
	For information on nesting conditions, see particularities of 
	GetResource. The service may be called from an ISR and from task level
/*/
StatusType ReleaseResource(ResourceType ResID) {
	StatusType stat = E_OK;

	if (ResID >= MAX_RESOURCE_ID) {
		pr_log("Error: %s() called with invalid ResID %d\n", __func__, ResID);
		return E_OS_ID;
	}

	if (!_OsResCtrlBlk[ResID].in_use) {
		pr_log("Warning: %s() is called for inactive ResID %d\n", __func__, ResID);
		return E_OS_RESOURCE;
	}
	_OsResCtrlBlk[ResID].in_use = false;

	/* Let us raise the priority of the calling task to ceiling priority */
	OsClrCeilingPrio();
	/* After this point, the calling task will be scheduled at the normal 
	priority (i.e., as configured in OSEK-Builder tool) by the FreeOSEK 
	kernel */

	return stat;
}