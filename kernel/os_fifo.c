#include <stdio.h>

#include "os_fifo.h"

#define OS_FIFO_TESTING	1


DefineFifoQueue(OS_QUEUE_WAITING, 100);
DefineFifoQueue(OS_QUEUE_SUSPENDED, 101);
DefineFifoQueue(OS_QUEUE_READY, 102);
DefineFifoQueue(OS_QUEUE_RUNNING, 103);


OsFifoType* FifoList[] = {
	&OS_FIFO(OS_QUEUE_WAITING),
	&OS_FIFO(OS_QUEUE_SUSPENDED),
	&OS_FIFO(OS_QUEUE_READY),
	&OS_FIFO(OS_QUEUE_RUNNING)
};

#ifdef OS_FIFO_TESTING
int main(void)
{
	int i;
	for (i = 0; i < OS_QUEUE_MAX; i++) {
		printf("size[%d] = %d\n", i, FifoList[i]->size);
	}
	for (i = 0; i < 106; i++) {
		OsFifoWrite(OS_QUEUE_WAITING, 2);
		OsFifoWrite(OS_QUEUE_RUNNING, 2);
	}
}
#endif


int OsFifoWrite(OsQueueType QueueID, TaskType TaskID) {
	u32 new_head;

	/* Validate inputs */
	if (TaskID >= TASK_ID_MAX) {
		printf("Error: %s() invalid TaskID: %d\n", __func__, TaskID);
		return -1;
	}
	if (QueueID >= OS_QUEUE_MAX) {
		printf("Error: %s() invalid QueueID: %d\n", __func__, QueueID);
		return -1;
	}
	new_head = (FifoList[QueueID]->head + 1) % FifoList[QueueID]->size;

	/* Push to FIFO head as long as the head does't eat the tail */
	if (new_head != FifoList[QueueID]->tail) {
#if (OS_FIFO_TESTING != 1)
		FifoList[QueueID]->task[new_head] = (OsTaskType*)&OsTaskList[TaskID];
#endif
		FifoList[QueueID]->head = (FifoList[QueueID]->head + 1) %
				(FifoList[QueueID]->size);
	}
	else {
		printf("Error: %s() FIFO (ID = %d) Full! Missing task = %d\n",
			__func__, QueueID, TaskID);
	}
}


int OsFifoRead(OsQueueType QueueID, TaskType TaskID) {
	if ((TaskID >= TASK_ID_MAX) || (QueueID >= OS_QUEUE_MAX)) {
		printf("Error: %s() invalid inputs\n", __func__);
		return -1;
	}

	/* Read from FIFO head, always, but don't go beyond tail */
}