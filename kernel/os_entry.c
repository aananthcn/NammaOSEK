#include <stdio.h>
#include <board.h>
#include <osek.h>

#include <os_api.h>
#include <os_fifo.h>

#include <sg_appmodes.h>
#include <sg_tasks.h>
#include <sg_fifo.h>


int OsAppMode;

int SetActiveApplicationMode(AppModeType mode) {
	if (mode >= OS_MODES_MAX) {
		printf("Error: Invalid mode! New mode %d > MAX (%d)\n", mode, OS_MODES_MAX);
		return -1;
	}

	OsAppMode = mode;
	return 0;
}

AppModeType GetActiveApplicationMode(void) {
	return OsAppMode;
}

void SetupScheduler(AppModeType mode) {
	int t, m, prio;
	OsFifoType* pFifo;

	// check all tasks marked as autostart
	for (t=0; t < TASK_ID_MAX; t++) {
		for (m=0; m < OsTaskList[t].n_appmodes; m++) {
			/* check if task 't' is configured to run in this mode */
			if (mode == *(((AppModeType*) OsTaskList[t].appmodes)+m)) {
				prio = OsTaskList[t].priority;
				if (prio < SG_FIFO_QUEUE_MAX_LEN) {
					pFifo = (OsFifoType*) ReadyQueue[prio];
					OsFifoWrite(pFifo, t);
				}
				else {
					printf("Error: task ID:%d has invalid priority: %d\n",
						 OsTaskList[t].id, prio);
				}
			}
		}
	}
	// add them to right fifo
	printf("%s:%s() under construction\n", __FILE__, __func__);
}

void StartOS(AppModeType mode) {
	SetActiveApplicationMode(mode);
	if (mode != OSDEFAULTAPPMODE) {
		printf("%s::%s(): Error: Invalid OS START mode!\n", __FILE__, __func__);
		printf("Info: Expected mode == %d mode, actual == %d!\n", 
			OSDEFAULTAPPMODE, mode);
		return;
	}
	
	SetupScheduler(mode);

	while (OsAppMode == OSDEFAULTAPPMODE) {
		// code your OSEK scheduling logic here
	}
}


/*
 * This funtion runs in interrupt context, hence keep things as minimal as possible
 */
void SystemTickISR(void) {
	if (OsHandleTicks())
		printf("Error: OsHandleTicks return errors!\n");
}