#ifndef ACN_OSEK_OS_FIFO_H
#define ACN_OSEK_OS_FIFO_H

#include <ostypes.h>
#include <sg_tasks.h>

typedef struct {
	OsTaskType** task;
	u32 head;
	u32 tail;
	const u32 size;
} OsFifoType;


typedef enum {
	OS_QUEUE_WAITING,
	OS_QUEUE_SUSPENDED,
	OS_QUEUE_READY,
	OS_QUEUE_RUNNING,
        OS_QUEUE_MAX
}OsQueueType;


#define OS_FIFO(x)              (x##_fifo)
#define OS_TASK_BUF(x, y)       (x##_buffer[y])
#define DefineFifoQueue(name, len) \
		OsTaskType* OS_TASK_BUF(name, len); \
		OsFifoType OS_FIFO(name) = { \
			.task = (OsTaskType**) &name##_buffer, \
			.head = 0, \
			.tail = 0, \
			.size = (u32) len \
		};

#endif


int OsFifoWrite(OsQueueType QueueID, TaskType TaskID);