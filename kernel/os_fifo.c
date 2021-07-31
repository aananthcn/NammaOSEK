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
}
#endif


int OsFifoAddTask(OsQueueType QueueID, TaskType TaskID) {
	u32 head, tail, size;

	if ((TaskID >= TASK_ID_MAX) || (QueueID >= OS_QUEUE_MAX)) {
		printf("Error: %s() invalid inputs\n", __func__);
		return -1;
	}

	head = FifoList[QueueID]->head;
	tail = FifoList[QueueID]->tail;
	size = FifoList[QueueID]->size;

	// Push to FIFO as long as the head doesn't eat the tail
	if (FifoList[QueueID]->head >= FifoList[QueueID]->tail) {
		FifoList[QueueID]->entry[FifoList[QueueID]->head] = TaskID;
		FifoList[QueueID]->head++;
		if (FifoList[QueueID]->head > FifoList[QueueID]->size) {
			FifoList[QueueID]->head = 0;
		}
	}
}