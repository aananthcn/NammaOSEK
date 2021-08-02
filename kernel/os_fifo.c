#include <stdio.h>
#include <sg_tasks.h>

#include "os_fifo.h"



DefineFifoQueue(OS_QUEUE_WAITING, OS_FIFO_SIZE);
DefineFifoQueue(OS_QUEUE_SUSPENDED, OS_FIFO_SIZE);
DefineFifoQueue(OS_QUEUE_READY, OS_FIFO_SIZE);
DefineFifoQueue(OS_QUEUE_RUNNING, OS_FIFO_SIZE);


OsFifoType* TaskQueues[] = {
	&OS_FIFO(OS_QUEUE_WAITING),
	&OS_FIFO(OS_QUEUE_SUSPENDED),
	&OS_FIFO(OS_QUEUE_READY),
	&OS_FIFO(OS_QUEUE_RUNNING)
};



int OsFifoWrite(OsQueueType QueueID, TaskType TaskID) {
	u32 head;

	/* Validate inputs */
	if ((TaskID < 0) || (TaskID >= TASK_ID_MAX)) {
		printf("Error: %s() invalid TaskID: %d\n", __func__, TaskID);
		return -1;
	}
	if ((QueueID < 0) || (QueueID >= OS_QUEUE_MAX)) {
		printf("Error: %s() invalid QueueID: %d\n", __func__, QueueID);
		return -1;
	}

	/* Push to FIFO head as long as the head does't eat the tail */
	if (!TaskQueues[QueueID]->full) {
		head = TaskQueues[QueueID]->head;
		TaskQueues[QueueID]->task[head] = (OsTaskType*)&OsTaskList[TaskID];
		TaskQueues[QueueID]->head = (TaskQueues[QueueID]->head + 1) %
				(TaskQueues[QueueID]->size);
		if (TaskQueues[QueueID]->head == TaskQueues[QueueID]->tail) {
			TaskQueues[QueueID]->full = true;
		}
	}
	else {
		printf("Error: %s() FIFO (ID = %d) Full! Missing task = %d\n",
			__func__, QueueID, TaskID);
	}
}


OsTaskType* OsFifoRead(OsQueueType QueueID) {
	OsTaskType *task = NULL;

	/* Validate input */
	if ((QueueID < 0) || (QueueID >= OS_QUEUE_MAX)) {
		printf("Error: %s() invalid QueueID: %d\n", __func__, QueueID);
		return NULL;
	}

	/* Read from FIFO tail, always, but don't go beyond head */
	if ((TaskQueues[QueueID]->tail != TaskQueues[QueueID]->head) ||
		(TaskQueues[QueueID]->full)) {
		task = TaskQueues[QueueID]->task[TaskQueues[QueueID]->tail];
		TaskQueues[QueueID]->full = false;
		TaskQueues[QueueID]->tail = (TaskQueues[QueueID]->tail + 1) %
				(TaskQueues[QueueID]->size);
	}
	else {
		printf("Error: %s() FIFO (ID = %d) Empty!\n", __func__, QueueID);
	}

	return task;
}