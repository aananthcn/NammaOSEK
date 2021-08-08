#include <stdio.h>
#include <sg_tasks.h>

#include "os_fifo.h"



int OsFifoWrite(OsFifoType* pFifoQ, TaskType TaskID) {
	u32 head;

	/* Validate inputs */
	if ((TaskID < 0) || (TaskID >= TASK_ID_MAX)) {
		printf("Error: %s() invalid TaskID: %d\n", __func__, TaskID);
		return -1;
	}
	if (pFifoQ == NULL) {
		printf("Error: %s() invalid pFifoQ\n", __func__);
		return -1;
	}

	/* Push to FIFO head as long as the head does't eat the tail */
	if (!pFifoQ->full) {
		head = pFifoQ->head;
		pFifoQ->task[head] = (OsTaskType*)&OsTaskList[TaskID];
		pFifoQ->head = (pFifoQ->head + 1) %
				(pFifoQ->size);
		if (pFifoQ->head == pFifoQ->tail) {
			pFifoQ->full = true;
		}
	}
	else {
		printf("Error: %s() FIFO Full! Missing task = %d\n",
			__func__, TaskID);
		return -1;
	}
#ifdef DEBUG
	printf("Debug Info: Adding task:%d to FIFO: %s\n", TaskID, pFifoQ->name);
#endif
	return 0;
}


OsTaskType* OsFifoRead(OsFifoType* pFifoQ) {
	OsTaskType *task = NULL;

	/* Validate input */
	if (pFifoQ == NULL) {
		printf("Error: %s() invalid pFifoQ\n", __func__);
		return NULL;
	}

	/* Read from FIFO tail, always, but don't go beyond head */
	if ((pFifoQ->tail != pFifoQ->head) ||
		(pFifoQ->full)) {
		task = pFifoQ->task[pFifoQ->tail];
		pFifoQ->full = false;
		pFifoQ->tail = (pFifoQ->tail + 1) %
				(pFifoQ->size);
	}
	else {
		printf("Error: %s() FIFO Empty!\n", __func__);
	}

	return task;
}
